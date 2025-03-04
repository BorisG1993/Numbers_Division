import csv
import re

input_file = "/home/borisg/Projects/NumbersDivision/gaps_pow_twos.txt"
output_file = "gaps_pow_twos.csv"

# Read input file and convert to CSV
with open(input_file, "r") as infile, open(output_file, "w", newline="") as outfile:
    writer = csv.writer(outfile)
    writer.writerow(["n", "k", "S", "pow2gap"])  # CSV header

    for line in infile:
        match = re.search(r"n: (\d+) k: (\d+) S: (\d+) pow2gap: (\d+)", line)
        if match:
            n, k, S, pow2gap = map(int, match.groups())
            writer.writerow([n, k, S, pow2gap])

print(f"CSV file saved as {output_file}")
