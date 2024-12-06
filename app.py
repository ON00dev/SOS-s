import os
from server.run import app

if __name__ == "__main__":
    # Define as configurações do ambiente para o Flask
    os.environ["FLASK_APP"] = "app.py"
    os.environ["FLASK_ENV"] = "development"  # Define o modo para desenvolvimento

    # Executa o servidor Flask
    app.run(debug=True, host="0.0.0.0", port=5000)
