# Cloud App — Motivational Quote Generator

A lightweight Python web application built with Flask and deployed on [Render](https://render.com) as a live, publicly accessible cloud service. The app dynamically serves a random motivational quote on each visit through a clean, styled web interface.

**Live URL:** https://dcc-lca-2-z925.onrender.com/

---

## Overview

This project demonstrates end-to-end deployment of a web application using the **Platform as a Service (PaaS)** model. The cloud provider (Render) handles all infrastructure, OS, and runtime concerns — the developer only supplies the application code and a couple of configuration files.

The backend is a Flask application that selects a random quote at request time and passes it to an HTML template via Jinja2. Gunicorn acts as the production WSGI server between Render's web layer and the Flask app.

---

## Tech Stack

| Layer | Technology |
|---|---|
| Backend | Python 3, Flask |
| Frontend | HTML, CSS (Jinja2 templating) |
| Production server | Gunicorn |
| Version control | Git / GitHub |
| Cloud platform | Render (PaaS) |

---

## Project Structure

```
Cloud/
├── app.py                 # Flask app — routing and quote logic
├── requirements.txt       # Python dependencies
├── Procfile               # Render start command
├── templates/
│   └── index.html         # Frontend HTML template
└── static/
    └── style.css          # Page styling
```

### File Reference

**`app.py`**
The application entry point. Initialises the Flask app, defines the root route, and selects a random motivational quote which is injected into the template on each request.

**`templates/index.html`**
The frontend view. Uses Jinja2 `{{ }}` syntax to render the dynamic quote passed from the Flask backend.

**`static/style.css`**
Stylesheet covering page layout, typography, colours, and button design.

**`requirements.txt`**
Declares the Python packages Render installs at build time:
```
flask
gunicorn
```

**`Procfile`**
Tells Render how to start the application in production:
```
web: gunicorn app:app
```

---

## Running Locally

**Prerequisites:** Python 3.x installed.

```bash
# 1. Clone the repository
git clone https://github.com/Awanti-Karmarkar/DCC-LCA-2.git
cd "DCC-LCA-2/Cloud App"

# 2. Install dependencies
pip install -r requirements.txt

# 3. Start the development server
python app.py
```

Open your browser at `http://127.0.0.1:5000` to see the app running locally.

---

## Deployment

The app is deployed on **Render** via GitHub integration. The steps taken were:

1. Developed and tested the Flask application locally.
2. Added `requirements.txt` and `Procfile` for cloud configuration.
3. Pushed the project to a GitHub repository.
4. Connected the repository to Render as a new Web Service.
5. Set build and start commands in the Render dashboard:
   - **Build command:** `pip install -r requirements.txt`
   - **Start command:** `gunicorn app:app`
6. Render built and deployed the app, generating a public URL.

Any subsequent `git push` to the connected branch triggers an automatic redeploy.

---

## Concepts Demonstrated

- **PaaS deployment** — the cloud provider abstracts servers, OS, and networking; the developer ships only application code
- **Local vs. cloud hosting** — Flask's built-in dev server (`flask run`) is single-threaded and not meant for public traffic; Gunicorn handles concurrent requests in production
- **Gunicorn as WSGI server** — bridges Render's HTTP layer and the Flask application object
- **Environment parity** — `requirements.txt` ensures the same packages are installed in every environment, local or cloud
