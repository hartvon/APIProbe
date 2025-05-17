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

@app.route('/echo', methods=['GET', 'POST', 'PUT', 'DELETE'])
def echo():
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
        'headers': headers,
        'query_params': args,
        'body': data,
        'time': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    }

    return jsonify(response)

@app.route('/api/users', methods=['GET'])
def get_users():
    users = [
        {'id': 1, 'name': 'Zhang San', 'email': 'zhangsan@example.com'},
        {'id': 2, 'name': 'Li Si', 'email': 'lisi@example.com'},
        {'id': 3, 'name': 'Wang Wu', 'email': 'wangwu@example.com'}
    ]
    return jsonify(users)

@app.route('/api/users/<int:user_id>', methods=['GET'])
def get_user(user_id):
    # Simulate database query
    users = {
        1: {'id': 1, 'name': 'Zhang San', 'email': 'zhangsan@example.com'},
        2: {'id': 2, 'name': 'Li Si', 'email': 'lisi@example.com'},
        3: {'id': 3, 'name': 'Wang Wu', 'email': 'wangwu@example.com'}
    }

    if user_id in users:
        return jsonify(users[user_id])
    else:
        return jsonify({'error': 'User not found'}), 404

@app.route('/api/users', methods=['POST'])
def create_user():
    if not request.is_json:
        return jsonify({'error': 'Request must be in JSON format'}), 400

    data = request.get_json()
    # Simulate user creation
    return jsonify({
        'id': 4,  # Simulate auto-generated ID
        'name': data.get('name', ''),
        'email': data.get('email', ''),
        'created_at': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    }), 201

@app.route('/api/users/<int:user_id>', methods=['PUT'])
def update_user(user_id):
    if not request.is_json:
        return jsonify({'error': 'Request must be in JSON format'}), 400

    data = request.get_json()
    # Simulate user update
    return jsonify({
        'id': user_id,
        'name': data.get('name', ''),
        'email': data.get('email', ''),
        'updated_at': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    })

@app.route('/api/users/<int:user_id>', methods=['DELETE'])
def delete_user(user_id):
    # Simulate user deletion
    return jsonify({
        'message': f'User {user_id} has been deleted',
        'deleted_at': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    })

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
