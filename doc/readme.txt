# How to build
1. Run build.bat
2. Once done and there were no errors, go to /build
3. Open /build/e_peso.exe

# Potential issues
* Only fully works on Windows since _getch and system("cls") might not work for other operating systems.
* Since there are only 8 digits in a reference number, the transaction and account id can go beyond this after 100 000 000 accounts or transactions. But the number is so high that this shouldn't be an issue
* The program doesn't save if you close the terminal externally instead of through the interface
* Saving could get corrupted if the program exits while saving or the power goes out?

# Notes
* Account names can be anything for now