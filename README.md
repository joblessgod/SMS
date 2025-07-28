# Student Management System `v2.2.0` (updated)

A simple, efficient command-line student management application built in C.

## Overview

SMS is a lightweight student management system that allows users to register, login, and manage student profiles through a clean terminal interface. Built with security and user experience in mind.

## Features

- **User Registration** - Create new student accounts with comprehensive validation
- **Secure Login System** - Password-protected access with masked password input
- **Data Persistence** - All user data is safely stored in local files
- **Input Validation** - Email format validation and phone number verification
- **Colorful Interface** - Enhanced terminal experience with color coding
- **Error Handling** - Robust error management with helpful user messages
- **Auto-Login** - Seamless experience after successful registration

## Quick Start

### Download and Run
1. Download `SMS.exe`
2. Double-click to run the application
3. Follow the on-screen menu options

### First Time Usage
1. Select option `1` for Sign Up
2. Enter your details when prompted
3. After successful registration, you'll be automatically logged in

## System Requirements

- Windows 7/8/10/11
- 1 MB available RAM
- 5 MB free disk space
- Command Prompt or PowerShell support

## Menu Options

```
================================
  Student Management System
================================
1. Sign Up
2. Login  
3. Exit
--------------------------------
Enter your choice (1-3):
```

### Option 1: Sign Up
Create a new student account by providing:
- Full Name (required)
- Email Address (must be valid format)
- Phone Number (7-15 digits only)
- Password (minimum 4 characters)
- Password Confirmation

### Option 2: Login
Access your existing account using:
- Registered email address
- Your account password

### Option 3: Exit
Safely terminate the application with a 3-second countdown

## Data Storage

User information is stored in `users.txt` in the following format:
```
FullName,Email,Phone,Role,Password
```

**Important:** Keep the `users.txt` file in the same directory as the executable to maintain your data.

## Input Validation

The system validates:

### Email Format
- Must contain `@` symbol
- Must have at least one dot after `@`
- Cannot be empty

### Phone Numbers
- Only numeric digits allowed
- Length between 7-15 characters
- No special characters or spaces

### Passwords
- Minimum 4 characters required
- Must match confirmation password
- Displayed as asterisks during input

## File Structure

```
SMS/
├── SMS.exe     # Main executable
├── users.txt          # User data (auto-created)
└── README.md          # This file
```

## Error Messages

Common error messages and solutions:

| Error | Cause | Solution |
|-------|--------|----------|
| "Email already exists" | Duplicate registration | Use different email address |
| "Invalid email format" | Incorrect email format | Include @ and proper domain |
| "Passwords do not match" | Confirmation mismatch | Re-enter passwords carefully |
| "Invalid phone number" | Wrong phone format | Use only digits, 7-15 length |
| "No users found" | Missing users.txt | Create account first (Sign Up) |

## Usage Examples

### Successful Registration
```
=== Create New Account ===
Enter your full name: John Doe
Enter your email: john.doe@email.com
Enter your phone number: 1234567890
Enter password: ****
Confirm password: ****

Success! Account created for John Doe as Student.
```

### Successful Login
```
=== Login to Your Account ===
Enter your email: john.doe@email.com
Enter your password: ****

Login successful! Welcome, John Doe.

=== Your Profile ===
Name:  John Doe
Email: john.doe@email.com
Phone: 1234567890
Role:  Student
```

## Compilation Instructions

For developers who want to compile from source:

### Prerequisites
- GCC compiler or equivalent
- Windows development environment

### Build Commands
```bash
# Single command compilation
gcc -o SMS.exe main.c helper.c mainMenu.c

# Alternative with explicit linking
gcc -c main.c helper.c mainMenu.c
gcc -o SMS.exe main.o helper.o mainMenu.o
```

### Source Files
- `main.c` - Main application logic and user interface
- `helper.h/helper.c` - Utility functions for input and system operations
- `mainMenu.h/mainMenu.c` - Menu display and navigation functions

## Security Features

- Password masking during input (displayed as asterisks)
- Input buffer clearing to prevent overflow attacks
- File operation error handling
- Memory-safe string operations with defined buffer limits

## Troubleshooting

### Application Won't Start
- Ensure you're running on a supported Windows version
- Check that the executable has proper permissions
- Verify sufficient disk space is available

### Cannot Login
- Verify email and password are correct
- Check that you've registered an account first
- Ensure `users.txt` file exists and is accessible

### Data Lost
- Check if `users.txt` exists in the application directory
- Restore from backup if available
- Register new accounts if data is unrecoverable

## Version History

### v2.2.0 (Current)
- Initial release
- Core registration and login functionality
- Input validation system
- File-based data storage
- Enhanced user interface

## Development Team

**Team Sapphire**
- Student Management System Development
- July 2025

## License

This project is provided as-is for educational and personal use.

## Support

For issues, questions, or feature requests:
1. Check this README for common solutions
2. Verify you're using the latest version
3. Report bugs with detailed error descriptions

## Contributing

To contribute to this project:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## Future Enhancements

Planned features for upcoming versions:
- Grade management system
- Course enrollment tracking
- Administrative user roles
- Data export capabilities
- Enhanced security measures
- GUI interface option

---

**Last Updated:** July 27, 2025  
**Version:** 2.2.0  
**Compatibility:** Windows 7+