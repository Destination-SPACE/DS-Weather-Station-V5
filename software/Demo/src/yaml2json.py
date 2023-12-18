import sys
import os
import yaml
import json

def convert_yaml_to_json(yaml_filename, json_filename):
    with open(yaml_filename, 'r') as yaml_file:
        yaml_data = yaml.safe_load(yaml_file)

    with open(json_filename, 'w') as json_file:
        json.dump(yaml_data, json_file, indent=2)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script_name.py <yaml_filename> <json_filename>")
        sys.exit(1)

    yaml_filename = sys.argv[1]
    json_filename = sys.argv[2]

    convert_yaml_to_json(yaml_filename, json_filename)
