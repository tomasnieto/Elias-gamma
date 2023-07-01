def compare_files(file1, file2):
    with open(file1, 'r') as f1, open(file2, 'r') as f2:
        lines1 = f1.readlines()
        lines2 = f2.readlines()

    if len(lines1) != len(lines2):
        return False

    for line_num, (line1, line2) in enumerate(zip(lines1, lines2), start=1):
        if line1.strip() != line2.strip():
            print(f"Difference found in line {line_num}:")
            print(f"File 1: {line1.strip()}")
            print(f"File 2: {line2.strip()}")
            return False

    return True


test_file = "test.txt"
out_file = "out.txt"

if compare_files(test_file, out_file):
    print("All lines are the same.")
else:
    print("Lines are not the same.")
