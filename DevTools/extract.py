import os
import re
from openpyxl import Workbook

def extract_functions_from_header(file_path):
    with open(file_path, 'r') as f:
        content = f.read()
        # Regular expression to capture access specifier, static keyword, return type, and function name.
        pattern = re.compile(r'(public|protected|private)?\s*(static)?\s*([\w\:\*\<\>]+)\s+([\w]+)\s*\(', re.MULTILINE)
        matches = pattern.findall(content)
        # Filter and transform the matches
        return [(m[0] or 'private', 'Yes' if m[1] else 'No', m[3]) for m in matches]

def main(folder_path):
    wb = Workbook()
    ws = wb.active
    wb.remove(ws)  # Removing the default sheet

    for root, dirs, files in os.walk(folder_path):
        for file in files:
            if file.endswith('.h'):
                file_path = os.path.join(root, file)
                functions = extract_functions_from_header(file_path)
                if functions:
                    ws = wb.create_sheet(title=file.replace('.h', ''))  # create a sheet with file name
                    ws.append(['accessor', 'static?', 'function name'])
                    for function in functions:
                        ws.append(function)

    wb.save("output.xlsx")

if __name__ == "__main__":
    path = input("Enter the path to the folder: ")
    main(path)
