from cs50 import get_int


def main():
    # Important vars
    s = " "
    p = "#"

    # Input Validation
    while True:
        h = get_int("Height: ")
        if h > 0 and h < 9:
            break

    # Draw the #
    for i in range(h):
        print(s[:1]*(h-(i+1)) + p[:1]*(i+1), end="  ")
        print(p[:1]*(i+1))


# Run Main
if __name__ == "__main__":
    main()