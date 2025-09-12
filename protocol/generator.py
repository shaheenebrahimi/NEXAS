import yaml
from pathlib import Path

class ArduinoProtocolGenerator:
    @staticmethod
    def get_dir():
        return "controller/"

    @staticmethod
    def get_header():
        return "// Auto-generated file. Do not edit manually.\n\n"

    @staticmethod
    def parse_constant(key, value: str):
        return f"#define {key} {value}\n"

    @staticmethod
    def parse_enum(key, value: dict):
        line = f"enum class {key} {{\n"
        line += ",\n".join([f"   {name} = {val}" for name, val in value.items()])
        line += "\n};\n"
        return line


class PythonProtocolGenerator:
    @staticmethod
    def get_dir():
        return "behaviors/"

    @staticmethod
    def get_header():
        return "# Auto-generated file. Do not edit manually.\n\n"

    @staticmethod
    def parse_constant(key, value: str):
        return f"{key} = {value}\n"

    @staticmethod
    def parse_enum(key, value: dict):
        line = f"class {key}:\n"
        line += "".join([f"   {name} = {val}\n" for name, val in value.items()])
        return line
    

def generate_protocol_files():
    
    with open("protocol/protocol.yaml", "r") as f:
        spec = yaml.safe_load(f)

    for pfile, generator in [("protocol.h", ArduinoProtocolGenerator), ("protocol.py", PythonProtocolGenerator)]:
        out_file = Path(generator.get_dir() + pfile)
        lines = {"constants": [], "enums": []}
        for key, value in spec.items():
            if isinstance(value, dict):
                lines['enums'].append(generator.parse_enum(key, value))
            else:
                lines['constants'].append(generator.parse_constant(key, value))

        with open(out_file, "w") as f:
            f.write(generator.get_header())
            f.write("".join(lines["constants"]))
            f.write("\n")
            f.write("".join(lines["enums"]))
        
        print(f"Wrote protocol to {out_file}")
