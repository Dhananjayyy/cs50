from nltk.tokenize import sent_tokenize
from cs50 import get_string, get_int


def lines(a, b):
    """Return lines in both a and b"""
    line_list = list()
    # Get lines from a and b
    a_lines = a.split("\n")
    b_lines = b.split("\n")

    line_list = [line if not line == " " else "" for line in a_lines if line in b_lines]

    # Use Dict from Keys to preserve order of list but remove duplicates
    return list(dict.fromkeys(line_list))


def sentences(a, b):
    """Return sentences in both a and b"""
    sent_list = list()
    # Get lines from a and b
    a_sent = sent_tokenize(a)
    b_sent = sent_tokenize(b)

    sent_list = [sent for sent in a_sent if sent in b_sent]
    return list(dict.fromkeys(sent_list))


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    sub_list = list()
    # Get lines from a and b
    a_sub = sub(a, n)
    b_sub = sub(b, n)

    sub_list = [sub for sub in a_sub if sub in b_sub]
    return list(dict.fromkeys(sub_list))


def sub(a, n):
    sub_string = list()
    l = len(a)
    for i in range(l - (n - 1)):
        sub_string.append(a[i: i + n])
    return list(dict.fromkeys(sub_string))

