# CyberTemp

A Python client library and service for interacting with the CyberTemp temporary email API. It enables you to generate disposable email addresses, monitor inboxes, and extract URLs or verification codes—ideal for testing or privacy-safe sign‑ups.
<img width="1728" height="886" alt="Screenshot 2025-07-29 at 00 14 37" src="https://github.com/user-attachments/assets/adef87e4-57a3-40d4-9278-5741a3d8e817" />


## 🚀 Features

- Fetch available disposable email domains  
- Get email contents by address or message ID  
- Search for emails by subject  
- Extract URLs from email bodies using regex  
- (Premium/API-key) Access rate‑limit removal, balance checks, private mailbox, delete emails/inbox  

## 📦 Installation and setup

Set WI-FI passwords in 


## 🧰 Quick Start

1) Deploy /api folder on your server with index.html file
2) Install firmware on ESP32 from /firmware folder and change links to api
3) Test it!

## ⚠️ Rate Limits

- **Free tier**: built-in 1-second delay between requests.  
- **Premium tier**: no enforced delay, pay-per-use model ([pypi.org](https://pypi.org/project/cybertemp/), [cybertemp.xyz](https://www.cybertemp.xyz/)).

## 🛠️ Setup

1. Clone or navigate to the repository at `AlexNoyanov/CyberTemp`.
2. Create and activate a Python virtual environment.
3. Run `pip install .` or use the published PyPI package with `pip install cybertemp`.
4. (Optional) Set your `API_KEY` if using premium features.
5. Use any of the client methods as shown above to interact with the API.

## 🧪 Testing

- Try creating a mailbox by visiting the service UI (e.g. `cybertemp.xyz`) to get a random temp email.
- Use the client to poll or fetch messages for that address.
- Ensure error handling for retries and invalid emails.

## 🤝 Contributing

Contributions are welcome. Please open issues or pull requests for bug fixes, feature requests, or improvements.

## 📄 License

MIT License (or as specified in the repo).
