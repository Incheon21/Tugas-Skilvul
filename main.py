from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/endpoint', methods=['POST'])
def receive_data():
    if request.is_json:
        data = request.get_json()
        temperature = data.get('temperature')
        humidity = data.get('humidity')
        
        print(f"Received data - Temperature: {temperature}C, Humidity: {humidity}%")
        
        response = {
            "status": "success",
            "message": "Data received successfully"
        }
        return jsonify(response), 200
    else:
        response = {
            "status": "error",
            "message": "Request must be JSON"
        }
        return jsonify(response), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
