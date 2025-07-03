import sys

def binary_to_c_array(input_file, output_file, array_name="sodl_binary_data"):
    try:
        with open(input_file, "rb") as bin_file:
            data = bin_file.read()

        with open(output_file, "w") as c_file:
            # Write the C header with array declaration
            c_file.write('#include "target_types.h"\n\n')
            c_file.write(f"const ehs_char {array_name}[] = {{\n    ")

            # Convert binary data to hex format
            hex_data = [f"0x{byte:02X}" for byte in data]
            
            # do this only when parsing from a buffer
            #hex_data.append("0x00")  # Add null terminator at the end
            
            # Write formatted output (16 bytes per line)
            for i, hex_value in enumerate(hex_data):
                if i % 16 == 0 and i != 0:
                    c_file.write("\n    ")
                c_file.write(f"{hex_value}, ")

            c_file.write("\n};\n\n")
            c_file.write(f"const ehs_uint32 {array_name}_size = sizeof({array_name}) - 1;\n")  # Exclude null terminator from size

        print(f"Successfully wrote binary data to {output_file} (null-terminated)")

    except FileNotFoundError:
        print(f"Error: The file {input_file} does not exist.")
    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage: python script.py input.bin output.c
if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python sodl_bin_to_c_file.py <input_binary> <output_c_file>")
    else:
        binary_to_c_array(sys.argv[1], sys.argv[2])

