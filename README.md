# Store-Receipt-Manager
Store Receipt Manager Written in Modern C++

![Screenshot](/images/screenshot.png)

---
## Store Receipt Manager User Guide

Welcome to the Store Receipt Manager! This guide will help you navigate and use the available commands to manage your store receipts.

### Available Commands:

1. **Set Store Details**
   - **Command**: `/set [detail] [value]`
   - **Description**: Set various details of the store receipt.
   - **Usage**:
     - `/set title [Store Title]`: Set the title of the store.
     - `/set branch [Branch Name]`: Set the branch name.
     - `/set street [Street Name]`: Set the street name.
     - `/set city [City Name]`: Set the city name.
     - `/set member [Member Number]`: Set the member number (numeric value only).
   - **Example**: `/set title My Grocery Store`

2. **Add an Item**
   - **Command**: `/additem [Item ID] [Item Name] [Item Price]`
   - **Description**: Add a new item to the receipt.
   - **Note**: Item ID should be 5 digits.
   - **Example**: `/additem 12345 Apples 2.50`

3. **Print the Receipt**
   - **Command**: `/print`
   - **Description**: Display the current receipt with all its details and items.

4. **Save the Receipt**
   - **Command**: `/save`
   - **Description**: Save the current receipt to a file named "receipt.json".

5. **Load a Receipt**
   - **Command**: `/load`
   - **Description**: Load a receipt from the file named "receipt.json".

6. **Apply Discount to an Item**
   - **Command**: `/discount [Item ID] [Discount Percentage]`
   - **Description**: Apply a discount to a specific item. The discount percentage should be provided as a decimal (e.g., 0.10 for 10%).
   - **Example**: `/discount 12345 0.20` (This will apply a 20% discount to the item with ID 12345)

7. **Exit the Program**
   - **Command**: `/exit`
   - **Description**: Exit the Store Receipt Manager.

### Tips:
- Always ensure that you provide the correct details when using the commands.
- If you encounter any errors or issues, carefully read the error message displayed. It will guide you on what went wrong.
- Remember to save your receipt after making changes to avoid losing any data.

---

Feel free to refer to this guide whenever you need assistance with the commands. Enjoy managing your store receipts!
