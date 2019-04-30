import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True

fields = ["name", "language", "experience"]


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    # Store Row Data in csv and redirect
    rowD = {}
    rowD["experience"] = request.form.get("exp")
    rowD["language"] = request.form.get("language")
    rowD["name"] = request.form.get("name")

    # Error Checking
    for key, value in rowD.items():
        if not value:
            return render_template("error.html", message="Empty " + key + " field")

    # Storing in append mode
    with open("survey.csv", "a") as db:
        writer = csv.DictWriter(db, fieldnames=fields)
        writer.writeheader()
        writer.writerow(rowD)

    return redirect("/sheet")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    # Obtain Row Data in csv and push to sheet.html
    data = []
    with open("survey.csv", "r") as db:
        reader = csv.DictReader(db)
        data = list(reader)
        # print (data)
    return render_template("sheet.html", data=data)
