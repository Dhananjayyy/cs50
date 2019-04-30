from cs50 import get_string
from sys import argv, exit


def main():
    # Check for incorrect usage
    if not len(argv) == 2:
        print("Usage: python bleep.py dictionary")
        exit(1)
    path = argv[1]

    # Open file and load file into dictionary
    # could also use dObj = set(d.readlines())
    d = open(path, "r")
    dObj = set()
    for line in d:
        word = line.strip("\n")
        dObj.add(word)

    # Obtain phrase and prepare censored string
    phrase = get_string("What message would you like to censor?\n")
    censor = ""

    words = phrase.split(" ")

    # Check if word in phrase is in dictionary
    for word in words:
        if word.lower() in dObj:
            word = "*" * len(word)
        censor = censor + " " + word

    print(censor.strip())


if __name__ == "__main__":
    main()
