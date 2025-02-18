from flask import Flask, request, jsonify, render_template
from twilio.rest import Client
import requests

# Configuração do Flask
app = Flask(
    __name__,
    static_folder="../app/static",  # Diretório de arquivos estáticos
    template_folder="../app/templates"  # Diretório de templates HTML
)

# Configuração do Twilio
TWILIO_ACCOUNT_SID = "ID DA CONTA TWILIO"
TWILIO_AUTH_TOKEN = "TOKEN DE AUTENTICAÇÃO"
TWILIO_PHONE_NUMBER = "NUMERO DO TWILIO"  # Número Twilio

# Cliente Twilio
twilio_client = Client(TWILIO_ACCOUNT_SID, TWILIO_AUTH_TOKEN)

# Lista para armazenar números cadastrados (em memória)
emergency_numbers = []

@app.route('/')
def index():
    """
    Rota principal: serve a página index.html.
    """
    return render_template('index.html')

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

    errors = []
    for recipient in emergency_numbers:
        try:
            # Envia SMS usando a API Twilio
            twilio_client.messages.create(
                body=message,
                from_=TWILIO_PHONE_NUMBER,
                to=recipient["phoneNumber"].replace(" ", "").replace("(", "").replace(")", "").replace("-", "").replace("+", "")
            )
        except Exception as e:
            errors.append({"phoneNumber": recipient["phoneNumber"], "error": str(e)})

    if errors:
        return jsonify({"success": False, "errors": errors}), 500
    return jsonify({"success": True, "message": "SOS enviados com sucesso!"}), 200

# Lista para armazenar números cadastrados (em memória)
emergency_numbers = []

# Mensagem predefinida (inicialmente vazia)
default_message = {"content": ""}

@app.route('/save_message', methods=['POST'])
def save_message():
    """
    Salva a mensagem predefinida no servidor.
    """
    data = request.get_json()
    message_content = data.get("content")

    if not message_content:
        return jsonify({"error": "Conteúdo da mensagem está vazio"}), 400

    # Salva ou atualiza a mensagem predefinida
    global default_message
    default_message["content"] = message_content

    return jsonify({"success": True, "message": "Mensagem salva com sucesso!"}), 200

@app.route('/get_message', methods=['GET'])
def get_message():
    """
    Retorna a mensagem predefinida salva.
    """
    return jsonify({"success": True, "message": default_message["content"]}), 200

