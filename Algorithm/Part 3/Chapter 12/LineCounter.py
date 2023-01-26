import os
import sys

excluded_dirs = ('.devcontainer', '.vscode', 'bin', 'build', 'data', 'dependency', 'dependencies', 'Question', 'Resource')
included_exts = ('.cpp', '.hpp', '.h', '.c', '.cu', '.cuh', '.glsl', '.py', '.sh', '.md', '.json', 'CMakeLists.txt')

def CountLinesInFile(filename: str) -> int:
    with open(filename, 'r', encoding='utf-8') as f:
        lines = len(f.readlines())
    return lines

def CountLines(path: str) -> int:
    total_files, total_lines = 0, 0
    for entry in os.scandir(path):
        # Omitting the files that are in excluded directories.
        if entry.name in excluded_dirs:
            continue
        # Recursively counting files in each subdirectories.
        if entry.is_dir():
            files, lines = CountLines(entry.path)
            total_files += files
            total_lines += lines
        # Counting files.
        if entry.is_file():
            if entry.name.endswith(included_exts):
                lines = CountLinesInFile(entry.path)
                print(entry.path, lines)
                total_files += 1
                total_lines += lines
    return total_files, total_lines

def main():
    if len(sys.argv) != 2:
        print("Usage: python LineCounter.py <path>")
        return
    print("Number of files {}, number of lines {}.".format(*CountLines(sys.argv[1])))

if __name__ == "__main__":
    main()
