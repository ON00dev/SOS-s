from flask import Flask, request, jsonify, render_template
import requests

# Configuração do Flask
app = Flask(
    __name__,
    static_folder="../app/static",  # Diretório de arquivos estáticos (CSS, JS)
    template_folder="../app/templates"  # Diretório de templates (HTML)
)

# Configuração da API SMS Brevo
BREVO_SMS_API_KEY = "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"
BREVO_SMS_API_URL = "https://api.brevo.com/v3/transactionalSMS/sms"

# Lista para armazenar números cadastrados
emergency_numbers = []

@app.route('/')
def index():
    """
    Rota principal: serve a página index.html.
    """
    return render_template('index.html')

@app.route('/sos', methods=['POST'])
def receive_sos():
    """
    Recebe as coordenadas GPS do Arduino e envia SMS para os números cadastrados.
    """
    data = request.get_json()
    gps_location = data.get("gps_location")

    if not gps_location or not emergency_numbers:
        return jsonify({"error": "Localização GPS ausente ou nenhum número cadastrado"}), 400

    latitude, longitude = gps_location.split(',')

    message = f"SOS! Necessitamos de ajuda. Coordenadas: Latitude {latitude}, Longitude {longitude}."

    headers = {
        "accept": "application/json",
        "content-type": "application/json",
        "api-key": BREVO_SMS_API_KEY
    }

    errors = []
    for recipient in emergency_numbers:
        payload = {
            "type": "transactional",
            "unicodeEnabled": False,
            "sender": recipient["responsibleName"],
            "recipient": recipient["phoneNumber"].replace(" ", "").replace("(", "").replace(")", "").replace("-", "").replace("+", ""),
            "content": message,
            "tag": "SOS"
        }

        response = requests.post(BREVO_SMS_API_URL, json=payload, headers=headers)

        if response.status_code != 201:
            errors.append({"phoneNumber": recipient["phoneNumber"], "error": response.json()})

    if errors:
        return jsonify({"success": False, "errors": errors}), 500
    return jsonify({"success": True, "message": "SOS enviados com sucesso!"}), 200

@app.route('/add_number', methods=['POST'])
def add_number():
    """
    Cadastra um número e nome no servidor.
    """
    data = request.get_json()
    phone_number = data.get("phoneNumber")
    responsible_name = data.get("responsibleName")

    if not phone_number or not responsible_name:
        return jsonify({"error": "Número ou nome do responsável ausente"}), 400

    # Verifica duplicação
    for entry in emergency_numbers:
        if entry["phoneNumber"] == phone_number:
            return jsonify({"error": "Número já cadastrado"}), 400

    # Adiciona número à lista
    emergency_numbers.append({"phoneNumber": phone_number, "responsibleName": responsible_name})
    return jsonify({"success": True, "message": "Número cadastrado com sucesso!"}), 200

@app.route('/list_numbers', methods=['GET'])
def list_numbers():
    """
    Retorna todos os números cadastrados.
    """
    return jsonify({"success": True, "numbers": emergency_numbers}), 200

@app.route('/delete_number', methods=['DELETE'])
def delete_number():
    """
    Exclui um número cadastrado no servidor.
    """
    data = request.get_json()
    phone_number = data.get("phoneNumber")

    if not phone_number:
        return jsonify({"error": "Número de telefone ausente"}), 400

    global emergency_numbers
    emergency_numbers = [entry for entry in emergency_numbers if entry["phoneNumber"] != phone_number]

    return jsonify({"success": True, "message": "Número excluído com sucesso!"}), 200

if __name__ == '__main__':
    app.run(debug=True)
