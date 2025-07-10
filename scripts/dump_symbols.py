import subprocess
import os

def main():
    file_path = input("Enter the path to the file to objdump (-t): ").strip()

    if not os.path.isfile(file_path):
        print(f"Error: '{file_path}' is not a valid file.")
        return

    try:
        result = subprocess.run(["objdump", "-t", file_path], check=True, text=True, capture_output=True)
        print("\n=== Symbol Table ===\n")
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        print("Error running objdump:")
        print(e.stderr)

if __name__ == "__main__":
    main()
