from flask import Flask, request, jsonify
import smtplib
from email.mime.text import MIMEText

app = Flask(__name__)

# 配置你的邮箱信息
SMTP_SERVER = "smtp.163.com" 
SMTP_PORT = 465
SENDER_EMAIL = "coderhw@163.com" # 你的邮箱
AUTH_CODE = "SJeUG36LpgVhhLtC" 

@app.route('/send_email', methods=['POST'])
def send_email():
    data = request.json
    target = data.get('email')
    code = data.get('code')
    
    msg = MIMEText(f"您的本次操作的验证码是：{code}，有效期5分钟。", 'plain', 'utf-8')
    msg['Subject'] = "系统验证码"
    msg['From'] = SENDER_EMAIL
    msg['To'] = target

    try:
        with smtplib.SMTP_SSL(SMTP_SERVER, SMTP_PORT) as server:
            server.login(SENDER_EMAIL, AUTH_CODE)
            server.sendmail(SENDER_EMAIL, [target], msg.as_string())
        return jsonify({"status": "success"}), 200
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=8888)