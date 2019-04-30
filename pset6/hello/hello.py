from cs50 import get_string


def main():
    # Ask Name and Say Hello
    print("What is your name?")
    name = get_string("")
    print(f"hello, {name}")


# Check where program is being run
if __name__ == "__main__":
    main()
