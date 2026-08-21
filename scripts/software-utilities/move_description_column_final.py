#!/usr/bin/env python3
import os
import re
from bs4 import BeautifulSoup
import glob
import shutil
import time

def move_description_column(html_file):
    """
    Move the description column to the 3rd position in the parameters table.
    """
    # Create a backup of the original file
    backup_file = html_file + '.bak'
    shutil.copy2(html_file, backup_file)
    
    try:
        with open(html_file, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        soup = BeautifulSoup(content, 'html.parser')
        
        # Find the parameters table
        param_table = soup.find('table', class_='table_params')
        if param_table:
            # Find the header row
            header_row = param_table.find('tr')
            if header_row:
                # Get all header cells
                header_cells = header_row.find_all('th')
                if len(header_cells) >= 6:
                    # Find the description column (last column)
                    desc_cell = header_cells[5]
                    
                    # Remove the description column from its current position
                    desc_cell.extract()
                    
                    # Insert it after the second column (index 1)
                    header_cells[1].insert_after(desc_cell)
            
            # Process all data rows
            data_rows = param_table.find_all('tr')[1:]  # Skip header row
            for row in data_rows:
                cells = row.find_all('td')
                if len(cells) >= 6:
                    # Find the description cell (last column)
                    desc_cell = cells[5]
                    
                    # Remove the description cell from its current position
                    desc_cell.extract()
                    
                    # Insert it after the second column (index 1)
                    cells[1].insert_after(desc_cell)
        
        # Save the modified file
        with open(html_file, 'w', encoding='utf-8') as f:
            f.write(str(soup))
        
        # Remove the backup file if everything was successful
        os.remove(backup_file)
        return True
    
    except Exception as e:
        print(f"Error processing {html_file}: {e}")
        # Restore from backup if there was an error
        if os.path.exists(backup_file):
            shutil.copy2(backup_file, html_file)
            os.remove(backup_file)
        return False

def find_help_html_files(root_dir):
    """Find all help.html files in the given directory and its subdirectories."""
    return glob.glob(os.path.join(root_dir, '**', 'docs', 'help.html'), recursive=True)

def main():
    # Get the base directory (parent of the scripts directory)
    base_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    root_dir = os.path.join(base_dir, "Common", "Components")
    
    if not os.path.exists(root_dir):
        print(f"Error: Directory not found: {root_dir}")
        print("Please run this script from the base directory of the project.")
        return
    
    # Find all help.html files
    help_files = find_help_html_files(root_dir)
    
    if not help_files:
        print(f"No help.html files found in {root_dir}")
        return
    
    print(f"Found {len(help_files)} help.html files")
    
    # Process each file
    success_count = 0
    for help_file in help_files:
        print(f"Processing {help_file}...")
        if move_description_column(help_file):
            success_count += 1
            print(f"Modified {help_file}")
        else:
            print(f"Failed to modify {help_file}")
    
    print(f"Successfully processed {success_count} out of {len(help_files)} files")

if __name__ == "__main__":
    main() 