from flask import Flask, render_template
import random

app = Flask(__name__)

quotes = [
    "Believe in yourself!",
    "Stay consistent, success will follow.",
    "Small steps every day matter.",
    "Dream big, work hard.",
    "Push yourself, no one else will."
]

@app.route('/')
def home():
    quote = random.choice(quotes)
    return render_template('index.html', quote=quote)

if __name__ == '__main__':
    app.run(debug=True)