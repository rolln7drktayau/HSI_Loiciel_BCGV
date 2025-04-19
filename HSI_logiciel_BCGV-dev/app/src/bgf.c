/**
 * \file bgf.c
 * \brief Implementation of BGF system.
 * \details Read and write serial frames of BGF from driver.
 * \author Melvyn MUNOZ
 */

/***** Includes **************************************************************/

#include "bgf.h"
#include "serial.h"
#include "bit_utils.h"
#include "log.h"

/***** Definitions ***********************************************************/

#define BGF_SERIAL_CHANNEL (11 - 1) /* Serial channel 11 */
#define BGF_SERIAL_FRAME_SIZE (2)	/* bytes */
#define BGF_NUM_MSG (5)

typedef struct
{
	uint8_t id;
	uint8_t flag;
} bgf_msg_t;

/***** Static variables ******************************************************/

/* Last saved messages sent by BCGV to BGF and last saved flags */
static bgf_msg_t bgf_msg[BGF_NUM_MSG];
static flag_t flag_position_light_saved = false;
static flag_t flag_crossing_light_saved = false;
static flag_t flag_highbeam_light_saved = false;
static flag_t flag_indic_right_saved = false;
static flag_t flag_indic_left_saved = false;

/***** Functions *************************************************************/

/**
 * \brief Check if the message received is same as last message sent.
 * \param msg_received : BGF message received
 * \return bool : the data in the received message are the same in the sended message
 */
bool bgf_check_msg_received(const bgf_msg_t *msg_received)
{
	bool same_msg = false;

	if (msg_received->id > BGF_NUM_MSG)
	{
		log_error("invalid message id (%u)", msg_received->id);
	}
	else if ((bgf_msg[msg_received->id - 1].id == msg_received->id) &&
			 (bgf_msg[msg_received->id - 1].flag == msg_received->flag))
	{
		same_msg = true;
	}

	return same_msg;
}

/**
 * \brief Set acknowledgement bit according to received message.
 * \param msg : BGF message
 */
void bgf_set_bit_ack(const bgf_msg_t *msg)
{
	bit_flag_t bit_flag_actual = get_bit_flag_bgf_ack();
	bit_flag_t bit_ack = 1;

	bit_ack = bit_ack << (msg->id - 1);
	if (msg->flag == 1)
	{
		set_bit_flag_bgf_ack(bit_flag_actual | bit_ack);
	}
	else
	{
		set_bit_flag_bgf_ack(CLEAR_BIT(bit_flag_actual, bit_ack));
	}
}

/**
 * \brief Set serial write buffer with the BGF message to send
 * \param msg_to_send : BGF message to_send
 */
void bgf_set_buffer_write(const bgf_msg_t *msg_to_send)
{
	serial_buffer_write[BGF_SERIAL_CHANNEL].serNum = BGF_SERIAL_CHANNEL + 1;
	serial_buffer_write[BGF_SERIAL_CHANNEL].frameSize = BGF_SERIAL_FRAME_SIZE;
	serial_buffer_write[BGF_SERIAL_CHANNEL].frame[0] = msg_to_send->id;
	serial_buffer_write[BGF_SERIAL_CHANNEL].frame[1] = msg_to_send->flag;
}

/**
 * \brief Write one serial message to driver.
 * \param drv_fd : Fd driver
 * \param msg_id : Message id to set and send
 * \param msg_flag : Message flag to set and send
 * \return bool : true if successfully write to driver, false otherwise
 */
bool bgf_write_msg(int32_t drv_fd, uint8_t msg_id, uint8_t msg_flag)
{
	int32_t ret = 0;

	if (msg_id > BGF_NUM_MSG)
	{
		log_error("invalid BGF message id (%u)", msg_id);
		return false;
	}

	bgf_msg[msg_id - 1].id = msg_id;
	bgf_msg[msg_id - 1].id = msg_flag;

	bgf_set_buffer_write(&bgf_msg[msg_id - 1]);
	ret = drv_write_ser(drv_fd, serial_buffer_write, 1);
	if (ret == DRV_ERROR)
	{
		log_error("error while writing BGF_%u message to driver", msg_id);
		return false;
	}

	return true;
}

int32_t bgf_read_frames(int32_t drv_fd)
{
	int32_t ret = 0;
	int32_t count = 0;
	uint32_t serial_data_len = 0;
	bgf_msg_t msg_received;
	bool same_msg = false;

	ret = drv_read_ser(drv_fd, serial_buffer_read, &serial_data_len);
	if (ret == DRV_ERROR)
	{
		log_error("error while reading from driver", NULL);
		return DRV_ERROR;
	}

	/* Try to pull all available BGF messages */
	while ((serial_data_len > 0) && (serial_buffer_read[BGF_SERIAL_CHANNEL].frameSize == BGF_SERIAL_FRAME_SIZE))
	{
		count++;
		msg_received.id = serial_buffer_read[BGF_SERIAL_CHANNEL].frame[0];
		msg_received.flag = serial_buffer_read[BGF_SERIAL_CHANNEL].frame[1];

		/* Check for acknowledgement */
		same_msg = bgf_check_msg_received(&msg_received);
		if (same_msg == true)
		{
			bgf_set_bit_ack(&msg_received);
			log_info("Bit set", NULL);
		}

		/* Continue reading next available message */
		ret = drv_read_ser(drv_fd, serial_buffer_read, &serial_data_len);
		if (ret == DRV_ERROR)
		{
			log_error("error while reading from driver", NULL);
			return DRV_ERROR;
		}
	}

	return count;
}

int32_t bgf_write_frames(int32_t drv_fd)
{
	bool success = false;
	int32_t errors = 0;
	flag_t flag_position_light_new = get_flag_position_light();
	flag_t flag_crossing_light_new = get_flag_crossing_light();
	flag_t flag_highbeam_light_new = get_flag_highbeam_light();
	flag_t flag_indic_right_new = get_flag_indic_right();
	flag_t flag_indic_left_new = get_flag_indic_left();

	/* Send serial message only if flags are different */
	if (flag_position_light_new != flag_position_light_saved)
	{
		success = bgf_write_msg(drv_fd, BCGV_BGF_MSG_ID_1, flag_position_light_new);
		if (success == false)
		{
			errors++;
		}
	}
	if (flag_crossing_light_new != flag_crossing_light_saved)
	{
		success = bgf_write_msg(drv_fd, BCGV_BGF_MSG_ID_2, flag_crossing_light_new);
		if (success == false)
		{
			errors++;
		}
	}
	if (flag_highbeam_light_new != flag_highbeam_light_saved)
	{
		success = bgf_write_msg(drv_fd, BCGV_BGF_MSG_ID_3, flag_highbeam_light_new);
		if (success == false)
		{
			errors++;
		}
	}
	if (flag_indic_right_new != flag_indic_right_saved)
	{
		success = bgf_write_msg(drv_fd, BCGV_BGF_MSG_ID_4, flag_indic_right_new);
		if (success == false)
		{
			errors++;
		}
	}
	if (flag_indic_left_new != flag_indic_left_saved)
	{
		success = bgf_write_msg(drv_fd, BCGV_BGF_MSG_ID_5, flag_indic_left_new);
		if (success == false)
		{
			errors++;
		}
	}

	return errors;
}
