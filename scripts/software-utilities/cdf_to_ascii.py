#!/usr/bin/env python3
"""
CDF to ASCII Art Converter

Converts Component Description Files (CDF) to ASCII art diagrams
showing the component's ports, parameters, and structure.
"""

import xml.etree.ElementTree as ET
import sys
from pathlib import Path


class Port:
    def __init__(self, element):
        self.description = element.findtext('Description', '')
        self.port_type = element.findtext('PortType', '')
        self.x = int(element.findtext('XCoordinate', 0))
        self.y = int(element.findtext('YCoordinate', 0))
        self.data_type = element.findtext('DataType', '')
        self.cname = element.findtext('CName', '')
        if not self.cname:
            self.cname = element.findtext('Name', '')
        # Extract function ID
        func_elem = element.find('.//Function/Function_ERT1_ID')
        self.function_id = func_elem.text if func_elem is not None else ''

    def is_left_side(self):
        """Input and Start ports are on the left"""
        return self.port_type in ['InputPort', 'StartPort', 'InternalPort']

    def is_event(self):
        """Start, Finish, and Internal ports are events"""
        return self.port_type in ['StartPort', 'FinishPort', 'InternalPort']

    def get_label(self):
        """Get the port label"""
        label = self.description or self.cname or ''
        return label.strip()

    def get_type_str(self):
        """Get data type string for display"""
        if self.data_type:
            return f"({self.data_type})"
        return ""


class CDFParser:
    def __init__(self, filepath):
        self.tree = ET.parse(filepath)
        self.root = self.tree.getroot()

    def get_component_name(self):
        """Get component class name"""
        return self.root.findtext('.//FBID/Class', 'Unknown')

    def get_description(self):
        """Get long description"""
        return self.root.findtext('.//Description/LongDescription', '')

    def get_menu_path(self):
        """Get menu path"""
        menu = self.root.find('.//Description/Menu')
        if menu is not None:
            parts = [menu.text] if menu.text and menu.text.strip() else []
            for submenu in menu.findall('.//Menu'):
                if submenu.text and submenu.text.strip():
                    parts.append(submenu.text.strip())
            return ' > '.join(parts)
        return ''

    def _calculate_block_from_ports(self):
        """Calculate block dimensions from port positions"""
        ports = self.root.findall('.//Ports/Port')
        if not ports:
            return {'type': 'Unknown', 'width': '40', 'height': '40', 'text': '', 'text_x': '', 'text_y': ''}

        # Find extremes of port positions
        min_x = float('inf')
        max_x = float('-inf')
        min_y = float('inf')
        max_y = float('-inf')

        for port in ports:
            x = int(port.findtext('XCoordinate', 0))
            y = int(port.findtext('YCoordinate', 0))
            min_x = min(min_x, x)
            max_x = max(max_x, x)
            min_y = min(min_y, y)
            max_y = max(max_y, y)

        # Calculate dimensions with 1-char border
        width = max(40, max_x - min_x + 2)
        height = max(40, max_y - min_y + 2)

        return {
            'type': 'Data_Processor',
            'width': str(width),
            'height': str(height),
            'text': self.get_component_name(),
            'text_x': str(width // 4),
            'text_y': str(height // 4)
        }

    def get_block_info(self):
        """Get block dimensions and text"""
        block = self.root.find('.//Block')
        if block is None:
            # Handle CDF files without Block element - calculate from ports
            return self._calculate_block_from_ports()

        info = {
            'type': block.findtext('Type', 'Unknown'),
            'width': block.findtext('Width', ''),
            'height': block.findtext('Height', ''),
            'text': block.findtext('Text', ''),
            'text_x': block.findtext('TextX', ''),
            'text_y': block.findtext('TextY', '')
        }
        return info

    def get_parameters(self):
        """Get all parameters"""
        params = []
        for param in self.root.findall('.//Parameters/Parameter'):
            p = {
                'name': param.findtext('Name', ''),
                'type': param.findtext('DataType', ''),
                'default': param.findtext('DefaultValue', ''),
                'min': param.findtext('MinValue', ''),
                'max': param.findtext('MaxValue', ''),
                'description': param.findtext('Description', ''),
                'display_x': param.findtext('.//DisplayOnWidget/x', ''),
                'display_y': param.findtext('.//DisplayOnWidget/y', '')
            }
            params.append(p)
        return params

    def get_ports(self):
        """Get all ports, excluding internal ports (coordinate -1, -1)"""
        ports = []
        for port_elem in self.root.findall('.//Ports/Port'):
            port = Port(port_elem)
            # Skip ports with coordinates -1, -1 (internal/non-visual ports)
            if port.x == -1 and port.y == -1:
                continue
            ports.append(port)
        return ports

    def get_all_ports(self):
        """Get ALL ports including internal ports (for documentation)"""
        ports = []
        for port_elem in self.root.findall('.//Ports/Port'):
            ports.append(Port(port_elem))
        return ports

    def get_functions(self):
        """Get all functions as a dictionary of {ERT1_ID: name}"""
        functions = {}
        for func in self.root.findall('.//Functions/Function'):
            name = func.findtext('name', '')
            ert_id = func.findtext('.//ID/ERT1_ID', '')
            if name and ert_id:
                functions[ert_id] = name
        return functions


class ASCIIRenderer:
    def __init__(self, cdf_parser):
        self.parser = cdf_parser
        self.ports = self.parser.get_ports()
        self.left_ports = sorted([p for p in self.ports if p.is_left_side()], key=lambda x: x.y)
        self.right_ports = sorted([p for p in self.ports if not p.is_left_side()], key=lambda x: x.y)
        # Get internal ports (coordinate -1, -1)
        all_ports = self.parser.get_all_ports()
        self.internal_ports = [p for p in all_ports if p.x == -1 and p.y == -1 and p.port_type == 'InternalPort']

    def render(self):
        """Render the complete ASCII diagram"""
        lines = []

        # Header
        comp_name = self.parser.get_component_name()
        description = self.parser.get_description()
        menu_path = self.parser.get_menu_path()
        block_info = self.parser.get_block_info()

        lines.append(f"{comp_name}")
        if description:
            lines.append(f"{description[:80]}")
        if menu_path:
            lines.append(f"Menu: {menu_path}")

        # Dimensions
        dim_parts = []
        if block_info['type']:
            dim_parts.append(f"Type: {block_info['type']}")
        if block_info['width']:
            dim_parts.append(f"Width: {block_info['width']}")
        if block_info['height']:
            dim_parts.append(f"Height: {block_info['height']}")
        if dim_parts:
            lines.append(' | '.join(dim_parts))
        lines.append('')

        # Calculate max label length for alignment
        left_max = max([len(p.get_label()) for p in self.left_ports], default=0)
        right_max = max([len(p.get_label() + p.get_type_str()) for p in self.right_ports], default=0)

        # Box dimensions
        label_width = max(left_max, 10)
        box_inner_width = max(25, len(block_info['text']) + 4)
        total_width = label_width + 2 + box_inner_width + 2 + right_max + 4

        # Build the diagram line by line
        # Merge ports from both sides by Y coordinate
        all_ports = []
        for p in self.left_ports:
            all_ports.append(('left', p))
        for p in self.right_ports:
            all_ports.append(('right', p))
        all_ports.sort(key=lambda x: x[1].y)

        # Group ports by Y coordinate (within a threshold)
        port_rows = []
        current_y = -999
        current_row = {'left': None, 'right': None}

        for side, port in all_ports:
            if abs(port.y - current_y) > 5:  # New row threshold
                if current_row['left'] or current_row['right']:
                    port_rows.append(current_row)
                current_row = {'left': None, 'right': None}
                current_y = port.y
            current_row[side] = port

        if current_row['left'] or current_row['right']:
            port_rows.append(current_row)

        # Top border
        lines.append(f"{' ' * (label_width + 2)}┌{'─' * box_inner_width}┐")

        # Add component text in the middle
        text_inserted = False
        mid_point = len(port_rows) // 2

        # Render each row
        for idx, row in enumerate(port_rows):
            line = self._render_port_row(row['left'], row['right'],
                                         label_width, box_inner_width, right_max)
            lines.append(line)

            # Insert component text label in middle
            if not text_inserted and idx >= mid_point - 1 and block_info['text']:
                text = block_info['text']
                padding = (box_inner_width - len(text)) // 2
                text_line = f"{' ' * (label_width + 2)}│{' ' * padding}{text}{' ' * (box_inner_width - padding - len(text))}│"
                lines.append(text_line)
                text_inserted = True

        # Bottom border
        lines.append(f"{' ' * (label_width + 2)}└{'─' * box_inner_width}┘")

        # Internal ports list (if any) - shown below the box
        if self.internal_ports:
            lines.append('')
            header = "Internal Events:"
            lines.append(f"{' ' * (label_width + 2)}{header}")
            for ip in self.internal_ports:
                lines.append(f"{' ' * (label_width + 2)}  • {ip.cname}")

        # Legend
        lines.append('')
        lines.append('Legend: ── Data | ►─ Event')

        # Parameters
        params = self.parser.get_parameters()
        if params:
            lines.append('')
            lines.append(f'**Parameters ({len(params)}):**')
            for i, p in enumerate(params, 1):
                param_line = f"- **{p['name']}:** {p['default']}"
                if p['type'] == 'I' and p['min'] and p['max']:
                    param_line += f" ({p['min']} to {p['max']})"
                elif p['type'] == 'B':
                    param_line += " (Bool)"
                elif p['type'] == 'S':
                    param_line += " (String)"
                elif p['type'] == 'R' or p['type'] == 'F':
                    param_line += " (Real)"
                if p['description']:
                    param_line += f" - {p['description'][:50]}"
                if p['display_x'] and p['display_y']:
                    param_line += f" [Display: {p['display_x']},{p['display_y']}]"
                lines.append(param_line)

        # Port summary
        lines.append('')
        lines.append('**Port Summary:**')
        lines.append(f"- **Left:**  {len(self.left_ports)} ports ({len([p for p in self.left_ports if p.is_event()])} events, {len([p for p in self.left_ports if not p.is_event()])} data)")
        lines.append(f"- **Right:** {len(self.right_ports)} ports ({len([p for p in self.right_ports if p.is_event()])} events, {len([p for p in self.right_ports if not p.is_event()])} data)")

        # Detailed port listing organized by function (includes internal ports)
        functions = self.parser.get_functions()
        if functions:
            lines.append('')
            lines.append('**Ports by Function:**')

            for func_id, func_name in sorted(functions.items()):
                func_ports = []
                for p in self.parser.get_all_ports():
                    if p.function_id == func_id:
                        func_ports.append(p)

                if func_ports:
                    lines.append(f"\n*{func_name}:*")

                    # Group by port type
                    inputs = [p for p in func_ports if p.port_type == 'InputPort']
                    outputs = [p for p in func_ports if p.port_type == 'OutputPort']
                    starts = [p for p in func_ports if p.port_type == 'StartPort']
                    finishes = [p for p in func_ports if p.port_type == 'FinishPort']
                    internals = [p for p in func_ports if p.port_type == 'InternalPort']

                    if starts:
                        for p in starts:
                            lines.append(f"  - **{p.cname}** (Start Event)")
                    if internals:
                        for p in internals:
                            lines.append(f"  - **{p.cname}** (Internal Event - callable from ISR/thread)")
                    if inputs:
                        for p in inputs:
                            lines.append(f"  - **{p.cname}** ({p.data_type}) - Input")
                    if outputs:
                        for p in outputs:
                            lines.append(f"  - **{p.cname}** ({p.data_type}) - Output")
                    if finishes:
                        for p in finishes:
                            lines.append(f"  - **{p.cname}** (Finish Event)")

        return '\n'.join(lines)

    def _render_port_row(self, left_port, right_port, left_width, box_width, right_width):
        """Render a single row with optional ports on each side"""
        # Left side
        if left_port:
            label = left_port.get_label()
            connector = '►─' if left_port.is_event() else '──'
            left_part = f"{label:>{left_width}}{connector}┤"
        else:
            left_part = f"{' ' * left_width}  │"

        # Middle (box)
        middle_part = ' ' * box_width

        # Right side
        if right_port:
            label = right_port.get_label()
            type_str = right_port.get_type_str()
            connector = '►─' if right_port.is_event() else '──'
            right_part = f"├{connector}{label} {type_str}"
        else:
            right_part = '│'

        return f"{left_part}{middle_part}{right_part}"


def main():
    if len(sys.argv) < 2:
        print("Usage: cdf_to_ascii.py <cdf_file>")
        print("\nExample:")
        print("  python3 cdf_to_ascii.py Common/Components/core/const_i1.cdf")
        sys.exit(1)

    cdf_file = sys.argv[1]

    if not Path(cdf_file).exists():
        print(f"Error: File not found: {cdf_file}")
        sys.exit(1)

    try:
        parser = CDFParser(cdf_file)
        renderer = ASCIIRenderer(parser)
        output = renderer.render()
        print(output)
    except Exception as e:
        print(f"Error processing CDF file: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == '__main__':
    main()
