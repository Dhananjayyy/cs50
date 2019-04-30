from cs50 import get_int


def checksum(n):
    # Important Variables
    ns = str(n)
    sumA = 0
    sumB = 0

    # Check if Odd or Even
    offset = 1 if len(ns) % 2 == 0 else 0

    # Implement actual checksum
    for i in range(len(ns)-1, -1, -1):
        curr = int(ns[i])
        if (i+offset) % 2 == 0:
            sumB = sumB + curr
        else:
            dbl = curr * 2
            ld = 0
            while not dbl == 0:
                ld = dbl % 10
                dbl = dbl // 10
                sumA = sumA + ld

    sum = sumA + sumB

    # Return
    if sum % 10 == 0:
        return True
    else:
        return False


def typec(n):
    ns = str(n)
    # Obtain first two digits    # Obtain first two digits
    l = len(ns)
    # Obtain first two digits
    dig = n // 10 ** (l-2)
    if l == 15:
        if dig == 34 or dig == 37:
            return "AMEX"
    elif l == 16:
        if dig == 51 or dig == 52 or dig == 53 or dig == 54 or dig == 55:
            return "MASTERCARD"
        # For VISA only check very first digit
        if dig // 10 == 4:
            return "VISA"
    elif l == 13:
        if dig // 10 == 4:
            return "VISA"
    return "INVALID"


def main():
    # Main Program
    n = get_int("Number: ")
    if not checksum(n):
        print("INVALID")
        return
    t = typec(n)
    print(t)


# Run Function Main
if __name__ == "__main__":
    main()
