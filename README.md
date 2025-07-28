# CyberTemp

A Python client library and service for interacting with the CyberTemp temporary email API. It enables you to generate disposable email addresses, monitor inboxes, and extract URLs or verification codes—ideal for testing or privacy-safe sign‑ups.
<img width="1728" height="886" alt="Screenshot 2025-07-29 at 00 14 37" src="https://github.com/user-attachments/assets/adef87e4-57a3-40d4-9278-5741a3d8e817" />


## 🚀 Features

- Fetch available disposable email domains  
- Get email contents by address or message ID  
- Search for emails by subject  
- Extract URLs from email bodies using regex  
- (Premium/API-key) Access rate‑limit removal, balance checks, private mailbox, delete emails/inbox  

## 📦 Installation

```bash
pip install cybertemp
```

or install a specific version:

```bash
pip install cybertemp==0.0.4
```

## 🧰 Quick Start

```python
from cybertemp import CyberTemp

# Free tier—with automatic built‑in delay
client = CyberTemp(debug=True)

# Or premium via API key
client = CyberTemp(api_key="YOUR_API_KEY")

# List available domains
domains = client.get_domains()

# Poll inbox—for example with retries
emails = client.get_email_content("test@cybertemp.xyz", max_retries=3, delay_between_retries=2.0)

# Get a single email by its ID
email = client.get_email_content_by_id("email_id_here")

# Find a mail by subject substring
mail_id = client.get_mail_by_subject(email="test@cybertemp.xyz", subject_contains="Verification", max_attempts=5, delay_between_retries=1.5)

# Extract a URL matching a regex pattern from an email
url = client.extract_url_from_message(email="test@cybertemp.xyz", subject_contains="Verification", url_pattern=r'https://[^\s<>"]+', max_attempts=5, delay_between_retries=1.5)

# (Premium only) Check your API balance
balance = client.get_balance()
```

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
