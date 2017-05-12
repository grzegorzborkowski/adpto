import os
import subprocess
import filecmp
import sys

def calculate_number_of_tests():
    files = os.listdir("testy/")
    return len(files)

def execute_test(test_number, file_name_in, file_name_out):
    res_name = file_name_out + ".tmp"
    expected_out = "testy/" + file_name_out

    command = "g++ -std=c++11 laser_solution.cpp && ./a.out < testy/" + file_name_in + " > " + res_name
    print(command)
    subprocess.call(command, shell=True)
    if filecmp.cmp(res_name, expected_out, shallow=False):
        print (test_number, ": OK")
    else:
        print(test_number, ": FAILED")

res = calculate_number_of_tests()
res = int(res / 2)
for test_number in range(1, res+1):
    execute_test(test_number, "test" + str(test_number) + ".in", "test" + str(test_number) + ".out")

if '-no_remove' not in sys.argv[1:]:
    rm_command = "rm *.tmp"
    subprocess.call(rm_command, shell=True)