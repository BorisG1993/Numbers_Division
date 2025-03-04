import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("gaps_pow_twos.csv")

df["ratio_nk"] = df["n"] / df["k"]

plt.figure(figsize=(8, 6))
plt.scatter(df["ratio_nk"], df["pow2gap"], alpha=0.6)
plt.xlabel("n/k")
plt.ylabel("pow2gap")
plt.title("Relation Between n/k and pow2gap")
plt.grid(True)

plt.savefig("nk_to_pow_two_relation.jpg", format="jpeg", dpi=300)
plt.close()

df["ratio_n"] = df["n"]

plt.figure(figsize=(8, 6))
plt.scatter(df["ratio_n"], df["pow2gap"], alpha=0.6)
plt.xlabel("n")
plt.ylabel("pow2gap")
plt.title("Relation Between n and pow2gap")
plt.grid(True)

plt.savefig("n_to_pow_two_relation.jpg", format="jpeg", dpi=300)
plt.close()

print("Plots saved as jpg")




