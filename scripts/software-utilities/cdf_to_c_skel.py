#!/usr/bin/env python3
"""
CDF to Skeleton C/H Generator

Generates skeleton C and header files from Component Description Files (CDF)
using API2 macros for eRT component development.

This is an alternative to the eclipse plugins tool (iCB - inxware Component Builder)
and can be used after LLM generation of CDFs or after modification of existing CDFs 
to compare skeleton and existing code to allow interactive meging of new/removed features.

"""

import xml.etree.ElementTree as ET
import sys
from pathlib import Path
import re


class CDFSkeletonGenerator:
    def __init__(self, cdf_path):
        self.cdf_path = Path(cdf_path)
        self.tree = ET.parse(cdf_path)
        self.root = self.tree.getroot()

        # Extract basic info
        self.class_name = self.root.findtext('.//FBID/Class', 'Unknown')
        self.fb_id = self.root.findtext('.//FBID/ERT1_ID', '0x0000')
        self.description = self.root.findtext('.//Description/LongDescription', '')

        # Convert class name to various formats
        self.fb_name_lower = self._class_to_snake_case(self.class_name)
        self.fb_name_upper = self.fb_name_lower.upper()
        self.prefix_upper = 'INX_' + self.fb_name_lower + '_ARG'

        # Parse ports and functions
        self.ports = self._parse_ports()
        self.functions = self._parse_functions()
        self.parameters = self._parse_parameters()

    def _class_to_snake_case(self, name):
        """Convert ClassName to class_name"""
        # Insert underscore before capitals (except first)
        s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
        # Insert underscore before capital followed by lowercase
        return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()

    def _parse_ports(self):
        """Parse all ports from CDF"""
        ports = []
        for port_elem in self.root.findall('.//Ports/Port'):
            port = {
                'description': port_elem.findtext('Description', ''),
                'type': port_elem.findtext('PortType', ''),
                'data_type': port_elem.findtext('DataType', ''),
                'cname': port_elem.findtext('CName', ''),
                'function_id': port_elem.findtext('.//Function/Function_ERT1_ID', '0')
            }
            ports.append(port)
        return ports

    def _parse_functions(self):
        """Parse functions from CDF"""
        functions = {}
        for func_elem in self.root.findall('.//Functions/Function'):
            name = func_elem.findtext('name', '')
            func_id = func_elem.findtext('.//ID/ERT1_ID', '0')
            functions[func_id] = name
        return functions

    def _parse_parameters(self):
        """Parse parameters from CDF"""
        params = []
        for param_elem in self.root.findall('.//Parameters/Parameter'):
            param = {
                'name': param_elem.findtext('Name', ''),
                'type': param_elem.findtext('DataType', ''),
                'default': param_elem.findtext('DefaultValue', ''),
                'description': param_elem.findtext('Description', '')
            }
            params.append(param)
        return params

    def _get_c_type(self, data_type):
        """Map CDF data type to C type"""
        type_map = {
            'I': 'ehs_sint32',
            'R': 'ehs_real',
            'F': 'ehs_real',
            'B': 'ehs_bool',
            'S': 'ehs_char*'
        }
        return type_map.get(data_type, 'ehs_sint32')

    def _get_api2_suffix(self, data_type):
        """Get API2 macro suffix for data type"""
        suffix_map = {
            'I': 'I',
            'R': 'R',
            'F': 'R',
            'B': 'B',
            'S': 'S'
        }
        return suffix_map.get(data_type, 'I')

    def _get_parser_function(self, data_type):
        """Get the incremental parser function for a data type"""
        parser_map = {
            'I': 'EhsGetSint32FromString',
            'R': 'EhsGetDoubleFromString',
            'F': 'EhsGetDoubleFromString',
            'B': 'EhsGetUint8FromString',
            'S': 'EhsGetStringFromString'
        }
        return parser_map.get(data_type, 'EhsGetSint32FromString')

    def generate_header(self):
        """Generate header file content"""
        lines = []

        # Copyright header
        lines.append('/***************************************************************')
        lines.append('* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved')
        lines.append('* You may use, distribute and modify this code under the terms')
        lines.append('* of the LGPLv3 license.')
        lines.append('****************************************************************/')
        lines.append('')

        # Include guard
        guard_name = f'INXWARE_{self.fb_name_lower}'
        lines.append(f'#ifndef {guard_name}')
        lines.append(f'#define {guard_name}')
        lines.append('#include "inx-component.h"')
        lines.append('')

        # Description
        if self.description:
            lines.append('/*****************************************************')
            lines.append(f' * {self.description}')
            lines.append(' *****************************************************/')
            lines.append('')

        # FB name and ID defines
        lines.append(f'#define INXWARE_FB_NAME_{self.fb_name_lower} "{self.fb_name_lower}"')
        lines.append(f'#define INXWARE_FB_ID_{self.fb_name_lower} {self.fb_id}')
        lines.append('')

        # Function declarations
        lines.append('/* Declaring the functions used here as public interface for static linking into EHS builds */')
        lines.append(f'EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION({self.fb_name_lower}); // this relates to C file identify function')
        lines.append(f'EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION({self.fb_name_lower}); // this relates to C-file init function')
        lines.append(f'EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION({self.fb_name_lower}); // this relates to C-file destroy function')

        # RUN function declarations for each start port and internal port
        for port in self.ports:
            if port['type'] == 'StartPort':
                func_name = self.functions.get(port['function_id'], 'run')
                lines.append(f'EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION({self.fb_name_lower}_{func_name}); //a function executed by a start port')
            elif port['type'] == 'InternalPort':
                func_name = self.functions.get(port['function_id'], 'run')
                lines.append(f'EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION({self.fb_name_lower}_{func_name}); //an internal function (callable from ISR/thread)')

        lines.append('/* declare the table for use by the blockref table */')
        lines.append(f'EHS_FB_FUNCTIONS({self.fb_name_lower})')
        lines.append('')
        lines.append(f'#endif /* {guard_name} */')
        lines.append('')

        return '\n'.join(lines)

    def generate_c_file(self):
        """Generate C file content"""
        lines = []

        # Copyright header
        lines.append('/***************************************************************')
        lines.append('* Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved')
        lines.append('* You may use, distribute and modify this code under the terms')
        lines.append('* of the LGPLv3 license.')
        lines.append('****************************************************************/')
        lines.append('')

        # Includes
        lines.append('//ICB HEADER MACRO START -- DO NOT ALTER')
        lines.append('#include "inx-parameters.h"')
        lines.append('#include "inx-component.h"')
        lines.append(f'#include "inx-{self.fb_name_lower}.h"')
        lines.append('//ICB HEADER MACRO END -- DO NOT ALTER')
        lines.append('')

        # State structure
        lines.append('//ICB STATE VAR MACRO START -- DO NOT ALTER')
        lines.append('/* My Component state data structure. - Use this in your code! */')
        lines.append('typedef struct')
        lines.append('{')
        if self.parameters:
            lines.append('    /* Parameters */')
            for param in self.parameters:
                c_type = self._get_c_type(param['type'])
                param_name = param['name'].replace(' ', '_')
                lines.append(f'    {c_type} {param_name}; // {param["description"][:50]}')
            lines.append('')
        lines.append('    /* Add your state variables here */')
        lines.append(f'}} inx_{self.fb_name_lower}_state_type;')
        lines.append('//ICB STATE VAR MACRO END -- DO NOT ALTER')
        lines.append('')

        # Function table
        lines.append('//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER')
        lines.append('/* Populate the data structure used by EHS and map the function names to strings identified in CDF */')
        lines.append(f'EHS_FB_FUNCTIONS_START({self.fb_name_lower})')
        lines.append('')

        # Add function entries for each start port and internal port
        func_id_counter = 0x01
        for port in self.ports:
            if port['type'] == 'StartPort':
                func_name = self.functions.get(port['function_id'], 'run')
                lines.append(f'EHS_FB_FUNCTION_ENTRY("{func_name}", {hex(func_id_counter)}, {self.fb_name_lower}_{func_name})')
                func_id_counter += 1
            elif port['type'] == 'InternalPort':
                func_name = self.functions.get(port['function_id'], 'run')
                lines.append(f'EHS_FB_FUNCTION_ENTRY("{func_name}", {hex(func_id_counter)}, {self.fb_name_lower}_{func_name}) // Internal event')
                func_id_counter += 1

        lines.append('')
        lines.append('EHS_FB_FUNCTIONS_END')
        lines.append('//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER')
        lines.append('')

        # Friendly labels
        lines.append('//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER')
        lines.append('/* Friendly labels for the run function data and event function argument enumerations */')
        lines.append('')

        # Group ports by function
        func_ports = {}
        for port in self.ports:
            if port['type'] in ['StartPort', 'FinishPort', 'InputPort', 'OutputPort']:
                func_id = port['function_id']
                if func_id not in func_ports:
                    func_ports[func_id] = {'inputs': [], 'outputs': [], 'finishes': []}

                if port['type'] == 'InputPort':
                    func_ports[func_id]['inputs'].append(port)
                elif port['type'] == 'OutputPort':
                    func_ports[func_id]['outputs'].append(port)
                elif port['type'] == 'FinishPort':
                    func_ports[func_id]['finishes'].append(port)

        # Generate labels for each function
        for func_id, ports_dict in func_ports.items():
            func_name = self.functions.get(func_id, 'run')
            lines.append(f'/* {func_name} */')

            # Input ports
            for idx, port in enumerate(ports_dict['inputs'], 1):
                cname = port['cname'] or port['description'].replace(' ', '_')
                lines.append(f'#define {self.prefix_upper}_{func_name}_{cname}_DI {idx}')

            # Output ports
            for idx, port in enumerate(ports_dict['outputs'], 1):
                cname = port['cname'] or port['description'].replace(' ', '_')
                lines.append(f'#define {self.prefix_upper}_{func_name}_{cname}_DO {idx}')

            # Finish ports
            for idx, port in enumerate(ports_dict['finishes'], 1):
                cname = port['cname'] or port['description'].replace(' ', '_')
                lines.append(f'#define {self.prefix_upper}_{func_name}_{cname}_EO {idx}')

            lines.append('')

        lines.append('//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER')
        lines.append('')

        # IDENTIFY function
        lines.append('//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER')
        lines.append('/**')
        lines.append(' * Identify the function block to EHS.')
        lines.append(' * This function provides access to:')
        lines.append(' *  EHS_FB_IDENTIFY_PARAMETERS - string containing parameter text')
        lines.append(' *  EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block\'s context')
        lines.append(' */')
        lines.append(f'EHS_FB_IDENTIFY_FUNCTION({self.fb_name_lower})')
        lines.append('{')
        lines.append(f'    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_{self.fb_name_lower}_state_type);')
        lines.append('}')
        lines.append('//ICB IDENTIFY FUNCTION MACRO END -- DO NOT ALTER')
        lines.append('')

        # INIT function
        lines.append('//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER')
        lines.append('/**')
        lines.append(' * Initialise the function block. Populate the context area for the function block.')
        lines.append(' * This function provides access to:')
        lines.append(' *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block')
        lines.append(' *  EHS_FB_INIT_PARAMETERS - string containing the parameter text')
        lines.append(' */')
        lines.append(f'EHS_FB_INIT_FUNCTION({self.fb_name_lower})')
        lines.append('{')
        lines.append('    ehs_bool bRet = EHS_TRUE; /* assume success */')
        lines.append(f'    inx_{self.fb_name_lower}_state_type* state = (inx_{self.fb_name_lower}_state_type*)EHS_FB_INIT_CONTEXT;')
        lines.append('')

        # Generate parameter parsing using incremental parser functions
        if self.parameters:
            lines.append('    /* Parse initialization parameters using incremental parser functions */')
            lines.append('    const char* pParams = EHS_FB_INIT_PARAMETERS;')

            for param in self.parameters:
                # Determine the appropriate parser function
                parser_func = self._get_parser_function(param['type'])
                param_name = param['name'].replace(' ', '_')

                # Generate the parsing line
                lines.append(f'    pParams = {parser_func}(&(state->{param_name}), pParams); // {param["description"][:40]}')

            lines.append('')
        else:
            lines.append('    /* No parameters to parse */')
            lines.append('')

        lines.append('    /* Add initialization code here */')
        lines.append('')
        lines.append('    return bRet;')
        lines.append('}')
        lines.append('//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER')
        lines.append('')

        # RUN functions for each start port and internal port
        for port in self.ports:
            if port['type'] in ['StartPort', 'InternalPort']:
                func_id = port['function_id']
                func_name = self.functions.get(func_id, 'run')
                is_internal = port['type'] == 'InternalPort'

                lines.append(f'//ICB FUNCTION {func_name} MACRO START -- DO NOT ALTER')
                lines.append('/**')
                lines.append(f' * Definition of {self.fb_name_lower}_{func_name}.')
                if is_internal:
                    lines.append(' * INTERNAL EVENT: This function can be called from external sources (ISR, threads, etc.)')
                lines.append(' * This function can access the object data using:')
                lines.append(' *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block')
                lines.append(' */')
                lines.append(f'EHS_FB_RUN_FUNCTION({self.fb_name_lower}_{func_name})')
                lines.append('{')
                lines.append(f'    inx_{self.fb_name_lower}_state_type* state = (inx_{self.fb_name_lower}_state_type*)EHS_FB_RUN_CONTEXT;')
                lines.append('')

                # Input ports section
                lines.append('    /* Example: Read input ports using API2 macros */')
                if func_id in func_ports and func_ports[func_id]['inputs']:
                    for port_in in func_ports[func_id]['inputs']:
                        cname = port_in['cname'] or port_in['description'].replace(' ', '_')
                        suffix = self._get_api2_suffix(port_in['data_type'])
                        label = f'{self.prefix_upper}_{func_name}_{cname}_DI'
                        lines.append(f'    // if (EHS_FB_IN_CONNECTED_API2({label}))')
                        lines.append(f'    // {{')
                        lines.append(f'    //     {self._get_c_type(port_in["data_type"])} value = EHS_FB_IN_{suffix}_API2({label});')
                        lines.append(f'    //     // Process input: {port_in["description"]}')
                        lines.append(f'    // }}')
                else:
                    lines.append('    // This function has no input ports')

                lines.append('')
                lines.append('    /* Add your implementation here */')
                lines.append('')

                # Output ports section
                lines.append('    /* Example: Write output ports using API2 macros */')
                if func_id in func_ports and func_ports[func_id]['outputs']:
                    for port_out in func_ports[func_id]['outputs']:
                        cname = port_out['cname'] or port_out['description'].replace(' ', '_')
                        suffix = self._get_api2_suffix(port_out['data_type'])
                        label = f'{self.prefix_upper}_{func_name}_{cname}_DO'
                        lines.append(f'    // if (EHS_FB_OUT_CONNECTED_API2({label}))')
                        lines.append(f'    // {{')
                        lines.append(f'    //     EHS_FB_OUT_{suffix}_API2({label}) = result_value;')
                        lines.append(f'    //     // Output: {port_out["description"]}')
                        lines.append(f'    // }}')
                else:
                    lines.append('    // This function has no output ports')

                lines.append('')

                # Finish events section
                lines.append('    /* Example: Trigger finish event */')
                if func_id in func_ports and func_ports[func_id]['finishes']:
                    finish_port = func_ports[func_id]['finishes'][0]
                    cname = finish_port['cname'] or finish_port['description'].replace(' ', '_')
                    label = f'{self.prefix_upper}_{func_name}_{cname}_EO'
                    lines.append(f'    // EHS_FB_FINISH({label});')
                else:
                    lines.append('    // This function has no finish event ports')

                lines.append('}')
                lines.append(f'//ICB FUNCTION {func_name} MACRO END -- DO NOT ALTER')
                lines.append('')

        # DESTROY function
        lines.append('//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER')
        lines.append('/**')
        lines.append(' * Destroy the function block. Clean up any resources.')
        lines.append(' */')
        lines.append(f'EHS_FB_DESTROY_FUNCTION({self.fb_name_lower})')
        lines.append('{')
        lines.append(f'    // inx_{self.fb_name_lower}_state_type* state = (inx_{self.fb_name_lower}_state_type*)EHS_FB_DESTROY_CONTEXT;')
        lines.append('    // Add cleanup code here if needed')
        lines.append('    return EHS_TRUE;')
        lines.append('}')
        lines.append('//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER')
        lines.append('')

        return '\n'.join(lines)


def find_diff_tool():
    """Find available diff/merge tool"""
    import shutil
    tools = ['meld', 'diff']
    for tool in tools:
        if shutil.which(tool):
            return tool
    return None


def add_to_build_config(output_dir, base_name, category):
    """Offer to add component to build configuration"""
    import subprocess

    # Find the components.mk file
    components_mk = output_dir / "components.mk"
    if not components_mk.exists():
        # Try parent directories
        for parent_level in range(1, 4):
            parent = output_dir
            for _ in range(parent_level):
                parent = parent.parent
            test_path = parent / "components.mk"
            if test_path.exists():
                components_mk = test_path
                break

    if not components_mk.exists():
        print(f"\nWarning: Could not find components.mk file")
        return

    # Check if already in build config
    with open(components_mk, 'r') as f:
        content = f.read()
        if base_name in content:
            print(f"\n{base_name} already appears in {components_mk}")
            return

    # Ask user if they want to add it
    response = input(f"\nAdd {base_name}.$(OBJ) to {components_mk}? [y/N]: ").strip().lower()
    if response == 'y':
        # Backup the file
        backup = components_mk.with_suffix('.mk.bak')
        subprocess.run(['cp', str(components_mk), str(backup)])

        # Add the object to the OBJECTS line
        with open(components_mk, 'r') as f:
            lines = f.readlines()

        # Find the last OBJECTS += line and add after it
        insert_idx = -1
        for i, line in enumerate(lines):
            if line.strip().startswith('OBJECTS +='):
                insert_idx = i

        if insert_idx >= 0:
            # Add after the last OBJECTS line
            new_line = f"OBJECTS += {base_name}.$(OBJ)\n"
            lines.insert(insert_idx + 1, new_line)

            with open(components_mk, 'w') as f:
                f.writelines(lines)

            print(f"✓ Added to {components_mk}")
            print(f"  Backup saved to {backup}")
        else:
            print(f"Warning: Could not find OBJECTS += line in {components_mk}")


def main():
    import subprocess
    import os

    if len(sys.argv) < 2:
        print("Usage: cdf_to_skeleton.py <cdf_file>")
        print("\nGenerates skeleton C and header files from a CDF file.")
        print("\nExample:")
        print("  python3 cdf_to_skeleton.py Common/Components/user/my_component.cdf")
        sys.exit(1)

    cdf_file = sys.argv[1]
    if not Path(cdf_file).exists():
        print(f"Error: File not found: {cdf_file}")
        sys.exit(1)

    try:
        generator = CDFSkeletonGenerator(cdf_file)

        # Generate output file names
        cdf_path = Path(cdf_file)
        base_name = f"inx-{generator.fb_name_lower}"
        output_dir = cdf_path.parent

        # Determine category (user, peripherals, core, etc.)
        category = output_dir.name

        h_file = output_dir / f"{base_name}.h"
        c_file = output_dir / f"{base_name}.c"

        # Check if files already exist
        files_exist = h_file.exists() or c_file.exists()

        if files_exist:
            # Files exist - create with _gen suffix and offer to diff
            print(f"Existing files detected. Creating generated versions with _gen suffix...")
            h_file_gen = output_dir / f"{base_name}_gen.h"
            c_file_gen = output_dir / f"{base_name}_gen.c"

            # Generate and write header file
            header_content = generator.generate_header()
            print(f"Writing: {h_file_gen}")
            with open(h_file_gen, 'w') as f:
                f.write(header_content)

            # Generate and write C file
            c_content = generator.generate_c_file()
            print(f"Writing: {c_file_gen}")
            with open(c_file_gen, 'w') as f:
                f.write(c_content)

            print("\nGenerated files created with _gen suffix.")

            # Find and offer to run diff tool
            diff_tool = find_diff_tool()
            if diff_tool:
                print(f"\nCompare files using {diff_tool}?")
                if h_file.exists():
                    response = input(f"  Compare {h_file.name} and {h_file_gen.name}? [Y/n]: ").strip().lower()
                    if response != 'n':
                        subprocess.run([diff_tool, str(h_file), str(h_file_gen)])

                if c_file.exists():
                    response = input(f"  Compare {c_file.name} and {c_file_gen.name}? [Y/n]: ").strip().lower()
                    if response != 'n':
                        subprocess.run([diff_tool, str(c_file), str(c_file_gen)])

                print(f"\nAfter reviewing, you can:")
                print(f"  - Merge changes manually")
                print(f"  - Replace old files: mv {base_name}_gen.{{c,h}} → {base_name}.{{c,h}}")
                print(f"  - Delete generated files: rm {base_name}_gen.{{c,h}}")
            else:
                print("\nNo diff tool (meld/diff) found. Manual comparison needed.")
                print(f"Generated files: {h_file_gen.name}, {c_file_gen.name}")

        else:
            # New files - create with normal names
            print(f"Creating new component files...")

            # Generate and write header file
            header_content = generator.generate_header()
            print(f"Writing: {h_file}")
            with open(h_file, 'w') as f:
                f.write(header_content)

            # Generate and write C file
            c_content = generator.generate_c_file()
            print(f"Writing: {c_file}")
            with open(c_file, 'w') as f:
                f.write(c_content)

            print("\n✓ Skeleton files created successfully!")

            # Offer to add to build configuration
            add_to_build_config(output_dir, base_name, category)

            print(f"\nNext steps:")
            print(f"1. Edit {c_file} to implement your component logic")
            print(f"2. Rebuild with 'make all'")

    except Exception as e:
        print(f"Error generating skeleton files: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == '__main__':
    main()
