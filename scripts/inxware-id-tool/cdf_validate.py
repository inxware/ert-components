import os
import sys
import re
import xml.etree.ElementTree as ET

def file_contains_text(file_path, text_to_find_array):
    try:
        with open(file_path, 'rb') as file:
            file_contents = file.read()
            Found=False
            for text_to_find in text_to_find_array:
                Found = text_to_find in file_contents
                if Found is False:
                    break
            return Found
    except FileNotFoundError:
        print(f"File not found: {file_path}")
        return False

def read_c_function_blocks(filename):
    with open(filename, 'r') as file:
        content = file.read()
    function_block_name=None
    function_blocks=dict()
    try:
        for line in content.split('\n'):
            if 'EHS_FB_FUNCTIONS_START' in line:
                function_block_name=re.findall(r'\((.*?)\)', line)[0]
                function_blocks[function_block_name]=dict()
            elif 'EHS_FB_FUNCTIONS_END' in line:
                function_block_name=None
                
            if function_block_name is not None:
                function=re.search(r'EHS_FB_FUNCTION_ENTRY\("([^"]+)", ([^,]+), ([^)]+)\)', line)
                if function is not None:
                    function_blocks[function_block_name][function.group(1)]=str(int(function.group(2),16)) # read hex as int
    except:
        print("FAILED to find function! (",filename,")")
    return function_blocks       

def extract_h_fb_identify_name(file_path,class_name_in_file):
    identify_name=None
    with open(file_path, 'rb') as file:
        content = file.read()
        found_class_name=False
        for line in content.split(b'\n'):
            if found_class_name is False:
                if class_name_in_file in line:
                    found_class_name=True
            else:
                match=re.search(r'EHS_FB_FUNCTIONS\((\w+)\)', str(line))
                if match is not None:
                    identify_name=match.group(1)
                    break    
    return identify_name
        
def map_cdf_class_to_fb_identify_name(cdf_class_name, h_file_paths):
    for h_file_path in h_file_paths:
        class_name_in_file=bytes('"' + cdf_class_name + '"','utf-8')
        if file_contains_text(h_file_path, [class_name_in_file]) is True:
            identify_name=extract_h_fb_identify_name(h_file_path,class_name_in_file)
            return [identify_name,cdf_class_name,h_file_path]
            
    return None

def search_files(directory, file_extension):
    file_paths = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(file_extension):
                file_path = os.path.join(root, file)
                file_paths.append(file_path)
    return file_paths
    
def get_file_names(file_paths):
    file_names = []
    for file_path in file_paths:
        file_name = os.path.basename(file_path)
        #print(file_name)
        file_names.append(file_name)
    return file_names
    
def get_unique_elemnets(array1, array2):
    # Combine the arrays and convert them to a set
    combined_set = set(array1 + array2)
    # Find the unique elements by subtracting the intersection from the combined set
    unique_elements = list(combined_set - (set(array1) & set(array2)))
    print(unique_elements)
    return unique_elements
    
    
class FunctionArgument:
    def __init__(self, id):
        self.id=id
        self.index='-1'
    
class NewPort:
    def __init__(self, type_name):
        self.type_name = type_name
        self.functions = dict()
        
    def append_function(self, ert_id, argument):
        self.functions[ert_id] = argument
            
    def print_all(self):
        print(self.functions)

class NewCdf:
    def __init__(self, file_name):
        self.file_name = file_name
        self.class_name = ""
        self.function_ids = dict()
        self.ports = dict()
        
    def append_function(self, ertid, name):
        self.function_ids[ertid] = name
        
    def get_function_name(self, ertid):
        return self.function_ids[ertid]
        
    def append_port(self, type_name, port):
        try:
            self.ports[type_name].append(port)
        except:
            self.ports[type_name] = []
            self.ports[type_name].append(port)
            
    def get_ports(self, type_name):
        return self.ports[type_name]
        
    def print_all(self):
        print("file name =", self.file_name)
        print("function IDs =", self.function_ids)
        for key in self.ports:
            print("port type =", key)
            for port in self.ports[key]:
                port.print_all()
    
def parse_new_cdf(xml_file):
    file_name = os.path.basename(xml_file)
    tree = ET.parse(xml_file)
    root = tree.getroot()
    cdf = NewCdf(file_name)
    try:
        # Extract function block class name
        cdf.class_name = root.find('FBID/Class').text
    except:
        pass
    try:
        # Extract function names and IDs
        functions = root.find('Functions')
        for function in functions.findall('Function'):
            name = function.find('name').text
            ertid = function.find('ID/ERT1_ID').text
            cdf.append_function(ertid, name)
    except:
        pass
    # Extract port information
    ports = root.find('Ports')
    try:
        for port in ports.findall('Port'):
            try:
                port_type = port.find('PortType').text
                new_port = NewPort(port_type)
                try:
                    for function in port.findall('Function'):
                        function_ertid = function.find('Function_ERT1_ID').text
                        function_argument = function.attrib.get('argument')
                        new_port.append_function(function_ertid, function_argument)
                except:
                    pass
                cdf.append_port(port_type, new_port)
            except:
                pass
    except:
        pass
            
    return cdf

class OldPort:
    def __init__(self, type_name):
        self.type_name = type_name
        self.functions = dict()
        
    def append_function(self, name, argument):
        arg=FunctionArgument(argument)
        self.functions[name] = arg
    
    def get_function(self, name):
        return self.functions[name]
    
    def print_all(self):
        for key in self.functions:
            print("[%s : %s, %s]" % (key, self.functions[key].id, self.functions[key].index))

class OldCdf:
    def __init__(self, file_name):
        self.file_name = file_name
        self.ports = dict()
        
    def append_port(self, type_name, port):
        try:
            self.ports[type_name].append(port)
        except:
            self.ports[type_name] = []
            self.ports[type_name].append(port)
            
    def get_ports(self, type_name):
        return self.ports[type_name]
        
    def print_all(self):
        print("file name =", self.file_name)
        for key in self.ports:
            print("port type =", key)
            for port in self.ports[key]:
                port.print_all()
                
def get_old_index(ports, function_key, arg):
    index = 1
    for port in ports:
        try:
            arg2 = port.get_function(function_key)
            if arg2.id < arg.id:
                index = index + 1
        except:
            pass
    return index

def parse_old_cdf(xml_file):
    file_name = os.path.basename(xml_file)
    tree = ET.parse(xml_file)
    root = tree.getroot()
    cdf = OldCdf(file_name)
    try:
        for port in root.findall('Port'):
            try:
                port_type = port.find('PortType').text
                old_port = OldPort(port_type)
                try:
                    for function in port.findall('FunctionName'):
                        function_name = function.text
                        function_argument = function.attrib.get('argument')
                        old_port.append_function(function_name, function_argument)
                except:
                    pass
                cdf.append_port(port_type, old_port)
            except:
                pass
    except:
        pass
    # index ports
    try:
        for port_key in cdf.ports:
            ports = cdf.get_ports(port_key)
            for port in ports:
                for function_key in port.functions:
                    arg = port.get_function(function_key)
                    arg.index = get_old_index(ports, function_key, arg)
                    #print("",arg.id, arg.index)
    except:
        pass
        
    return cdf

def compare_port(new_port, old_port, new_func_name_map):
    try:
        for ert_id in new_port.functions:
            function_arg = new_port.functions[ert_id]
            function_name = new_func_name_map[ert_id]
            old_port_arg = old_port.get_function(function_name)
            if "InputPort" == new_port.type_name:
                if int(old_port_arg.id) is not int(function_arg):
                    print("[%s,%s] %s=%s" % (function_name, ert_id, function_arg, old_port_arg.id) )
            else:
                if int(old_port_arg.index) is not int(function_arg):
                    print("[%s,%s] %s=%s(%s)" % (function_name, ert_id, function_arg, old_port_arg.index, old_port_arg.id) )
    except Exception as e:
        print("Failed to process new port = ", e)
        
    
def compare_cdf(new_cdf, old_cdf, port_type):
    if port_type in new_cdf.ports:
        old_ports = old_cdf.get_ports(port_type)
        new_ports = new_cdf.get_ports(port_type)
        if len(old_ports) == len(new_ports):
            for i, new_port in enumerate(new_ports):
                old_port = old_ports[i]
                compare_port(new_port, old_port, new_cdf.function_ids)
        else:
            print("%s count is NOT equal in %s" % (port_type, new_ports.file_name) )
    else:
        #print("%s not used." % port_type)
        pass

def cdf_ports_compare(argv):
    if len(sys.argv) < 3:
        print("Please provide two directory paths as arguments.")
        sys.exit(1)
    # Directory to search for files
    directory1 = sys.argv[1] # latest cdf with ERT1 function ID mapping
    directory2 = sys.argv[2] # old cdf without mapping
    # File extension to search for
    file_extension = '.cdf'

    found_file_paths1 = search_files(directory1, file_extension)
    found_file_paths2 = search_files(directory2, file_extension)

    found_file_names1 = get_file_names(found_file_paths1)
    found_file_names2 = get_file_names(found_file_paths2)
    
    get_unique_elemnets(found_file_names1, found_file_names2)
    
    new_cdfs = dict()
    old_cdfs = dict()
    
    # get all new cdf
    for file_path in found_file_paths1:
        cdf = parse_new_cdf(file_path)
        new_cdfs[cdf.file_name] = cdf
        #cdf.print_all()
    
    # get all old cdf
    for file_path in found_file_paths2:
        cdf = parse_old_cdf(file_path)
        old_cdfs[cdf.file_name] = cdf
        #cdf.print_all()
        
    for file_name in found_file_names1:
        try:
            print("=================== %s =====================" % file_name)
            new_cdf = new_cdfs[file_name]
            old_cdf = old_cdfs[file_name]
            compare_cdf(new_cdf, old_cdf, "FinishPort")
            #compare_cdf(new_cdf, old_cdf, "OutputPort")
            #compare_cdf(new_cdf, old_cdf, "InternalPort")
            #compare_cdf(new_cdf, old_cdf, "StartPort")
            #compare_cdf(new_cdf, old_cdf, "InputPort")
        except Exception as e:
            print("Failed to process =", file_name, e)
            
def validate_cdf_and_c_code_function_id(directory):
    print("Validating function ID and c code in this directory=%s" % directory)
    
    found_cdf_file_paths = search_files(directory, ".cdf")
    found_h_file_paths = search_files(directory, ".h")
    found_c_file_paths = search_files(directory, ".c")
    
    # find all function blocks in c
    found_functions=[]
    for c_file_path in found_c_file_paths:
        if file_contains_text(c_file_path, [b'EHS_FB_FUNCTIONS_START']) is True:
            functions=read_c_function_blocks(c_file_path)
            found_functions.append([c_file_path,functions])
    # find all CDFs
    found_cdfs=[]
    for cdf_file_path in found_cdf_file_paths:
        cdf=parse_new_cdf(cdf_file_path)
        mapped_cdf_class=map_cdf_class_to_fb_identify_name(cdf.class_name,found_h_file_paths)
        fb_identify_name=None
        if mapped_cdf_class is not None:
            fb_identify_name=mapped_cdf_class[0]
            if fb_identify_name is None:
                print("Failed to map fb_identify_name for '",cdf.class_name,"' in ",mapped_cdf_class[2])
            #print(fb_identify_name)
        else:
            print("Failed to find fb_identify_name for ",cdf.class_name)
        found_cdfs.append([cdf_file_path,cdf,fb_identify_name])
    
    for found_cdf in found_cdfs:
        cdf=found_cdf[1]
        fb_identify_name=found_cdf[2]
        function=None
        for found_function in found_functions:
            if fb_identify_name in found_function[1]:
                function=found_function[1]
                break
        if function is not None:
            if bool(function[fb_identify_name]) == True:
                cdf_function_ids = dict((v,k) for k,v in cdf.function_ids.items())
                #print("FUNCTION=",function[fb_identify_name])
                #print("CDF=",cdf_function_ids)
                if function[fb_identify_name] != cdf_function_ids:
                    print("DIFFERENT (",fb_identify_name,")=",function[fb_identify_name],") | cdf(",cdf.class_name,")=",cdf_function_ids)
            else:
                print("EMPTY (",fb_identify_name,"), cdf(",cdf.class_name,")=",cdf.function_ids)
        else:
            print("Failed to find function for ",cdf.class_name)
        
if __name__ == "__main__":
    
    #cdf_ports_compare(sys.argv)
    
    validate_cdf_and_c_code_function_id("./Common/Components")
    
    