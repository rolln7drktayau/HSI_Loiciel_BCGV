# HSI LOGICIEL BCGV - CODE FILES GENERATOR

## Project Description

This project includes three key files for generating and managing project structure:
- `app_struct.xlsx`: Contains tables "TYPES" and "DONNEES" defining the project structure
- `gen_script.py`: Generates necessary files in `./app/lib/bcgv_api/include` and ./app/lib/bcgv_api/src, including `bcgv_api.h` and `bcgv_api.c`
- `requirements.txt`: Lists Python dependencies for the project

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Installation

### Prerequisites
Ensure you have the following packages installed:
- pandas
- openpyxl

### Setup Virtual Environment
To create and activate a virtual environment:

#### On Windows:
```cmd
# Create virtual environment
python -m venv .venv

# Activate the environment
venv\Scripts\activate
```

#### On macOS/Linux:
```bash
# Create virtual environment
python3 -m venv .venv

# Activate the environment
source venv/bin/activate
```

### Install Dependencies if it isn't automatically done
Dependencies will be automatically installed. You can skip this part.
Install the required packages:
```sh
pip install -r requirements.txt
```

## Usage
To run the project:
```sh
python gen_script.py
```

## Contributing
Contributors:
- Raphael CAUSSE
- Melvyn MUNOZ
- Roland Cédric TAYO

## License
INEM - 2024-2025

## Notes
- Ensure you are in the virtual environment before running the script
- If you encounter any issues, verify your Python and package versions
