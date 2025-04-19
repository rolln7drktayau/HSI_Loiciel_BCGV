import os
import subprocess
import sys

# Function that installs packages
def install_packages():
    try:
        import pandas
        import openpyxl
    except ImportError:
        subprocess.check_call([sys.executable, '-m', 'pip', 'install', '-r', 'requirements.txt'])

# Check and install packages if needed
install_packages()

import pandas as pd
import re

# Output directories
include_dir = '../app/lib/bcgv_api/include'
src_dir = '../app/lib/bcgv_api/src'
os.makedirs(include_dir, exist_ok=True)
os.makedirs(src_dir, exist_ok=True)

# A function that generates global variables
def create_global_variables(domaine, nom):
    match = re.search(r'\[(\d+); (\d+)\]', str(domaine))
    if match:
        min_val, max_val = match.groups()
        var_name = nom.upper().replace("_T", "")
        if min_val == '0':
            return f"#define {var_name}_MAX ({max_val})\n\n"
        else:
            return f"#define {var_name}_MIN ({min_val})\n#define {var_name}_MAX ({max_val})\n\n"
    return ""

# [bcgv_api.h] - A function that generates bcgv_api.h
def generate_bcgv_api_h(types_df, donnees_df):
    header = """/**
 * \\file bcgv_api.h
 * \\brief Type definitions and context functions for project
 * \\details Contains all custom types, enumerations, and context initialization/accessor functions used in the project
 * \\author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#ifndef BCGV_API_H
#define BCGV_API_H

#include <stdint.h>
#include <stdbool.h>

"""
    global domain_values
    domain_values = ''.join(types_df.apply(lambda row: create_global_variables(row['Domaine'], row['Nom']), axis=1))
    bcgv_api_h = header + f"// [Domain values]\n{domain_values}"
    
    for _, row in types_df.iterrows():
        commentaire, genre, declaration, nom = row['Commentaire'], row['Genre'].lower(), row['Declaration'], row['Nom']
        bcgv_api_h += f"\n// {commentaire}\n"
        if genre == 'atom':
            bcgv_api_h += f"typedef {declaration} {nom};\n"
        elif genre == 'enum':
            enum_values = ',\n    '.join(map(str.strip, declaration.split(',')))
            bcgv_api_h += f"typedef enum {{\n    {enum_values},\n}} {nom};\n"
    
    bcgv_api_h += """\n/**
 * \\brief Initialize context.
 * \\brief Initialize context variables for the api.
 */
void bcgv_ctx_init();
"""
    for _, row in donnees_df.iterrows():
        type_name, type_def = row['Nom'], row['Type']
        bcgv_api_h += f"""\n/**
 * \\brief Gets the {type_name.lower()} value.
 * \\details Returns the current state of the {type_name.lower()}.
 * \\return {type_def} : The {type_name.lower()} value.
 */
{type_def} get_{type_name.lower()}();

/**
 * \\brief Sets the {type_name.lower()} value.
 * \\details Sets the {type_name.lower()} to the given value.
 * \\param value : The new value for the {type_name.lower()}.
 */
void set_{type_name.lower()}({type_def} value);
"""
    bcgv_api_h += "\n#endif // BCGV_API_H"
    
    with open(os.path.join(include_dir, 'bcgv_api.h'), 'w') as file:
        file.write(bcgv_api_h)

# [bcgv_api.c] - A function that generates bcgv_api.c
def generate_bcgv_api_c(donnees_df, domain_values):
    bcgv_api_c = """/**
 * \\file bcgv_api.c
 * \\brief Context initialization and definitions for project
 * \\details Contains initialization and definition functions for all custom types and enumerations used in the project
 * \\author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include "bcgv_api.h"

// Context structure
typedef struct {
"""
    for _, row in donnees_df.iterrows():
        bcgv_api_c += f"    {row['Type']} {row['Nom']}; // {row['Commentaire']}\n"
    
    bcgv_api_c += """} context_t;

// Global context structure instance
static context_t context;

void bcgv_ctx_init() {
"""
    for _, row in donnees_df.iterrows():
        bcgv_api_c += f"    context.{row['Nom']} = {row['Valeur d\'init']};\n"
    bcgv_api_c += "}\n\n"
    
    # getters et setters
    for _, row in donnees_df.iterrows():
        type_name, type_def = row['Nom'], row['Type']
        var_name = type_name.upper().replace("_T", "")
        bcgv_api_c += f"""
{type_def} get_{type_name.lower()}() {{
    return context.{type_name.lower()};
}}

void set_{type_name.lower()}({type_def} value) {{
"""
        if f"#define {var_name}_MIN" in domain_values and f"#define {var_name}_MAX" in domain_values:
            bcgv_api_c += f"    if (value >= {var_name}_MIN && value <= {var_name}_MAX) {{\n"
            bcgv_api_c += f"        context.{type_name.lower()} = value;\n    }}\n"
        elif f"#define {var_name}_MAX" in domain_values:
            bcgv_api_c += f"    if (value <= {var_name}_MAX) {{\n"
            bcgv_api_c += f"        context.{type_name.lower()} = value;\n    }}\n"
        else:
            bcgv_api_c += f"    context.{type_name.lower()} = value;\n"
        bcgv_api_c += "}\n"

    with open(os.path.join(src_dir, 'bcgv_api.c'), 'w') as file:
        file.write(bcgv_api_c)

# Read the excel file
file_path = 'app_types_data.xlsx'
df = pd.read_excel(file_path, sheet_name=0)

# Extract "TYPES" & "DONNEES" data
types_start = df.apply(lambda row: 'TYPES' in row.values, axis=1).idxmax()
donnees_start = df.apply(lambda row: 'DONNEES' in row.values, axis=1).idxmax()
types_df = df.iloc[types_start+2 : donnees_start].dropna(how='all').reset_index(drop=True)
donnees_df = df.iloc[donnees_start+2 :].dropna(how='all').reset_index(drop=True)

# Dataframes column naming
types_df.columns = df.iloc[types_start+1]
donnees_df.columns = df.iloc[donnees_start+1]

# Generate files
generate_bcgv_api_h(types_df, donnees_df)
generate_bcgv_api_c(donnees_df, domain_values)

print(f"bcgv_api.h generated in {include_dir}.")
print(f"bcgv_api.c generated in {src_dir}.")
