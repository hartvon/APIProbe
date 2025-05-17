from flask import Flask, request, jsonify
import datetime

app = Flask(__name__)

@app.route('/', methods=['GET'])
def index():
    return jsonify({
        'message': 'Welcome to API Test Server',
        'status': 'running',
        'time': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    })

@app.route('/echo', defaults={'subpath': ''}, methods=['GET', 'POST', 'PUT', 'DELETE'])
@app.route('/echo/<path:subpath>', methods=['GET', 'POST', 'PUT', 'DELETE'])
def echo(subpath):
    # Get request information
    method = request.method
    headers = dict(request.headers)
    args = dict(request.args)

    # Get request body
    data = {}
    if request.is_json:
        data = request.get_json()
    elif request.form:
        data = dict(request.form)

    # Build response
    response = {
        'method': method,
        'path': subpath,
        'headers': headers,
        'query_params': args,
        'body': data,
        'time': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    }

    return jsonify(response)

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
