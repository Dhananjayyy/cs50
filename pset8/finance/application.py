import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Obtain transaction history of user
    portfolio = db.execute(
        "SELECT symbol, SUM(shares) total_shares FROM transactions WHERE user_id=:sid GROUP BY symbol", sid=session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id=:sid", sid=session["user_id"])[0]["cash"]

    # From transaction Dict make Dict containing Data for Table in index.html
    for comp in portfolio:
        info = lookup(comp["symbol"])
        comp["name"], comp["price"] = info["name"], info["price"]
        comp["tp"] = comp["price"] * comp["total_shares"]

    return render_template("index.html", disp=portfolio, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Obtain data from user
        info = lookup(request.form.get("symbol"))
        try:
            q = int(request.form.get("shares"))
        except ValueError:
            return apology("Non-Integers not Allowed")

        # Validate the input
        if info is None:
            return apology("Symbol Not Found")
        if q <= 0:
            return apology("Negative Values Not Allowed")

        # Check if user has enough money
        tp = q * info["price"]
        balance = db.execute("SELECT cash FROM users WHERE id=:sid", sid=session["user_id"])[0]["cash"]
        if balance < tp:
            return apology(f"Balance too Low for Shares of {info['symbol']} ")

        # Update Database to Deduct Balance
        db.execute("UPDATE users SET cash=:ucash WHERE id=:sid", ucash=balance-tp, sid=session["user_id"])

        # Update Database to Add Transaction
        db.execute("INSERT INTO transactions(user_id, symbol, shares, price) VALUES(:sid, :symbol, :share, :price)",
                   sid=session["user_id"], symbol=info["symbol"], share=q, price=info["price"])

        # Redirect if successful
        flash("Bought!")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    username = request.args.get("username")
    if len(username) > 0:
        if len(db.execute("SELECT id FROM users WHERE username=:username", username=username)) == 0:
            return jsonify(True)
    return jsonify(False)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    trans = db.execute("SELECT symbol, shares, price, time FROM transactions WHERE user_id=:sid", sid=session["user_id"])
    return render_template("history.html", trans=trans)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        info = lookup(request.form.get("symbol"))
        if info is None:
            return apology("Symbol Not Found")
        return render_template("quoted.html", info=info)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Check if POST req.
    if request.method == "POST":
        # Obtain username and password From User
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username and password are Not Blank
        if len(username.strip()) == 0 or len(password) == 0 or len(confirmation) == 0:
            return apology("Username or Password Field is Empty")

        # Ensure password is entered correctly
        if password != confirmation:
            return apology("Passwords Do Not Match")

        # Check if username is in DB
        if len(db.execute("SELECT * FROM users WHERE username = :username", username=username)) != 0:
            return apology("Username Already Exists")

        # Add username and password (hash) to the database
        db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)",
                   username=username, hash=generate_password_hash(password))

        # Redirect to index
        return redirect("/")

    # Render page if GET req.
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Check if post req.
    if request.method == "POST":
        # Get form data
        sym = request.form.get("symbol")
        try:
            q = int(request.form.get("shares"))
        except ValueError:
            return apology("You Must Own Stocks to Sell Em")

        # Get Current Price of Share for Computation
        curr_price = lookup(sym)["price"]

        # Validate the symbol input
        total_shares = db.execute("SELECT SUM(shares) total FROM transactions WHERE user_id=:sid AND symbol=:sym",
                                  sid=session["user_id"], sym=sym)
        if sym is None:
            return apology("Must Select A Symbol")
        if len(total_shares) == 0 or total_shares[0]["total"] < q:
            return apology("You Must Own Stocks to Sell Em")

        # Validate the share input
        if q <= 0:
            return apology("Shares Must be A Non Zero, Non Negative Integer")

        # Update both databases
        db.execute("UPDATE users SET cash = cash + :remv WHERE id=:sid", remv=q*curr_price, sid=session["user_id"])
        db.execute("INSERT INTO transactions(user_id, symbol, shares, price) VALUES(:sid, :sym, :shares, :price)",
                   sid=session["user_id"], sym=sym, shares=-q, price=curr_price)

        flash("Sold!")
        return redirect("/")
    else:
        # Get Companies Whose Share User Owns from DB
        comp = db.execute("SELECT symbol FROM transactions WHERE user_id=:sid GROUP BY symbol", sid=session["user_id"])

        return render_template("sell.html", comp=comp)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
