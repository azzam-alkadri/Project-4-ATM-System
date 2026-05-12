# 🏧 ATM Management System (C++)

## 📌 Overview
A console-based application that simulates a real-life ATM machine experience. It allows registered clients to securely log in and perform essential banking transactions. The system is designed with a focus on data integrity and security features.

---

## 🚀 Main Features
- **Secure Login System**: Protected access using Account Number and PinCode.
- **Login Protection**: Automatic program lockout after **3 failed login attempts** for security.
- **Quick Withdraw**: Fast withdrawal options for fixed amounts (20, 50, 100, 200, 400, 600, 800, 1000).
- **Normal Withdraw**: Custom withdrawal amounts (must be a multiple of 5).
- **Deposit**: Easily add funds to the account.
- **Check Balance**: Real-time balance inquiry.
- **Change PinCode**: Securely update your account PIN during the session.
- **Data Persistence**: All transactions and PIN updates are saved automatically to a `Clients.txt` file.

---

## ⚙️ How it Works
- **File Database**: Uses a text file as a permanent database to store and retrieve client records.
- **Record Mapping**: Converts text lines into Structured Data (Structs) for easy processing.
- **Validation Logic**: Validates withdrawal amounts against the current balance before processing.
- **Confirmation Flow**: Every financial transaction requires a "Yes/No" confirmation from the user to ensure accuracy.

---

## ⚠️ Challenges & Solutions
- **Security Lockout**: Implemented a trial-based loop in the login screen to prevent unauthorized access.
- **Data Syncing**: Fixed logic errors where the screen balance would update even if the user cancelled the transaction. Now, memory updates only after successful file writes.
- **Transaction Accuracy**: Ensured that withdrawals only accept multiples of 5, mimicking real ATM hardware constraints.

---

## 🎯 What I Learned
- Building robust **Security Protocols** (Trials and Exit conditions).
- Managing **File I/O** operations in C++ (Loading, Saving, and Updating records).
- Separating **Business Logic** (Calculations) from **UI Logic** (Menus and Screens).
- Professional state management using global objects like `CurrentClient`.

---

## 💼 Project Purpose
A practical project that shows how to handle banking transactions safely and keep user data synced between the program and text files.


---

