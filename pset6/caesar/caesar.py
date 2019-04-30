from sys import argv, exit
from cs50 import get_string


def main():
    # Handle lack of argv
    if not len(argv) == 2:
        print("Usage: python caesar.py k")
        exit(1)

    # Store cipher code
    k = int(argv[1])

    # Input User
    txt = get_string("plaintext: ")
    cip = ""
    for c in txt:
        if c.isalpha():
            sub = 65 if c.isupper() else 97
            c = chr((ord(c) - sub + k) % 26 + sub)
        cip += c
    print("ciphertext: {}".format(cip))


if __name__ == "__main__":
    main()