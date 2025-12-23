#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

// Global settings (control conversion behavior for settings menu)
int stepVisual = 1;    // Step visualization: 1=Enabled, 0=Disabled
int numType = 0;       // Number type: 0=Unsigned integer, 1=Signed integer
int prefixShow = 1;    // Result prefix annotation: 1=Enabled, 0=Disabled

// Character mapping table (for 10-15 corresponding to A-F)
const char MAP[] = "0123456789ABCDEF";

// Function declarations (modular design for maintainability)
void clearBuffer();                // Clear input buffer
void getUserInput(char *input, int maxLen);  // Safely get user input
void showMainMenu();               // Display main menu
int selectBase(const char *type);  // Select source/target base
int inputValidation(const char *input, int base);  // Validate input against base rules
long long strToDecimal(const char *input, int base);  // Any base ¡ú Decimal (intermediate state)
void decimalToBase(long long num, int base, char *result);  // Decimal ¡ú Any base
void showDecimalToBaseSteps(long long num, int base);  // Step visualization for Decimal¡úTarget Base
void showBinaryToHexSteps(const char *binary);  // Step visualization for Binary¡úHexadecimal
void showBinaryToOctSteps(const char *binary);  // Step visualization for Binary¡úOctal
void showResult(const char *input, int src_base, int dest_base);  // Format and display results
void showSettings();               // Display settings menu
int isOverflow(const char *input); // Check if input overflows long long range

int main() {
    system("cls || clear");  // Clear screen (cross-platform compatible)
    printf("=====================================\n");
    printf("  ?? Amazing Base Converter v1.0 ?? \n");
    printf("=====================================\n");
    printf("  Supports conversion between 2-16 bases with step visualization\n");
    printf("=====================================\n\n");

    int choice;
    while (1) {
        showMainMenu();
        scanf("%d", &choice);
        clearBuffer();  // Clear buffer to avoid residual input issues

        switch (choice) {
            case 1: {  // Base conversion function
                system("cls || clear");
                // 1. Select source and target bases
                int src_base = selectBase("Source");
                int dest_base = selectBase("Target");

                // Validate if source and target bases are the same
                while (src_base == dest_base) {
                    printf("? Source base is the same as target base, no conversion needed!\n");
                    dest_base = selectBase("Target");
                }

                // 2. Input value to convert
                char input[50];
                printf("\n=====================================\n");
                printf("  ?? Enter Value to Convert\n");
                printf("=====================================\n");
                printf("Current conversion: [%d-base] ¡ú [%d-base]\n", src_base, dest_base);
                printf("Input Rules:\n");
                printf("- Only 0-9, A-F (uppercase) allowed; no spaces/symbols\n");
                printf("- Value must follow %d-base rules (e.g., binary only allows 0/1)\n", src_base);
                printf("=====================================\n");
                while (1) {
                    printf("Enter %d-base value: ", src_base);
                    getUserInput(input, 50);

                    // Validate input: base compliance + overflow check
                    if (!inputValidation(input, src_base)) {
                        printf("? Value does not follow %d-base rules! Please re-enter\n", src_base);
                        continue;
                    }
                    if (isOverflow(input)) {
                        printf("? Value too large! Exceeds calculation range, please re-enter\n");
                        continue;
                    }
                    break;  // Input is valid, exit loop
                }

                // 3. Display conversion results
                printf("\n? Converting...\n");
                showResult(input, src_base, dest_base);

                // 4. Ask to continue conversion
                char continueChoice[10];
                printf("\nContinue conversion? (1=Yes, 0=Return to main menu): ");
                getUserInput(continueChoice, 10);
                if (strcmp(continueChoice, "0") == 0) {
                    system("cls || clear");
                    printf("?? Returning to main menu...\n\n");
                } else {
                    system("cls || clear");
                    printf("?? Preparing new conversion...\n\n");
                }
                break;
            }
            case 2: {  // Conversion settings function
                system("cls || clear");
                showSettings();
                break;
            }
            case 3: {  // Exit program
                system("cls || clear");
                printf("=====================================\n");
                printf("  ?? Thank You for Using\n");
                printf("=====================================\n");
                printf("  Base Converter exited safely, welcome back next time!\n");
                printf("=====================================\n");
                return 0;
            }
            default: {  // Handle invalid input
                system("cls || clear");
                printf("? Invalid input! Please enter an integer between 1-3\n\n");
                break;
            }
        }
    }
}

// Clear input buffer (solve newline residue issue)
void clearBuffer() {
    while (getchar() != '\n');
}

// Safely get user input (limit length, remove newline)
void getUserInput(char *input, int maxLen) {
    fgets(input, maxLen, stdin);
    input[strcspn(input, "\n")] = '\0';  // Replace newline with null terminator
}

// Display main menu
void showMainMenu() {
    printf("=====================================\n");
    printf("  ?? Main Menu\n");
    printf("=====================================\n");
    printf("  1. Base Conversion (2-16 bases)\n");
    printf("  2. Conversion Settings\n");
    printf("  3. Exit Program\n");
    printf("=====================================\n");
    printf("Enter function number (1-3): ");
}

// Select source/target base (type: "Source" or "Target")
int selectBase(const char *type) {
    int choice, base;
    char input[20];
    printf("=====================================\n");
    printf("  ?? Select %s Base\n", type);
    printf("=====================================\n");
    printf("  Quick select common bases:\n");
    printf("  1. Binary (2-base)     2. Octal (8-base)\n");
    printf("  3. Decimal (10-base)   4. Hexadecimal (16-base)\n");
    printf("  5. Custom Base (2-16)\n");
    printf("=====================================\n");
    printf("Enter option (1-5): ");

    // Process base selection input (ensure integer)
    while (1) {
        getUserInput(input, 20);
        choice = atoi(input);
        if (choice >= 1 && choice <= 5) {
            break;
        } else {
            printf("? Invalid input! Please enter an integer between 1-5: ");
        }
    }

    // Parse selected base
    switch (choice) {
        case 1: base = 2; break;
        case 2: base = 8; break;
        case 3: base = 10; break;
        case 4: base = 16; break;
        case 5: {  // Custom base (2-16)
            while (1) {
                printf("Enter custom base (integer between 2-16): ");
                getUserInput(input, 20);
                base = atoi(input);
                if (base >= 2 && base <= 16) {
                    break;
                } else {
                    printf("? Base out of range! Please enter an integer between 2-16\n");
                }
            }
            break;
        }
        default: base = 10;  // Default to decimal (theoretically unreachable)
    }

    printf("? %s base selected: %d-base\n\n", type, base);
    return base;
}

// Validate if input value complies with target base rules (base: 2-16)
int inputValidation(const char *input, int base) {
    int len = strlen(input);
    if (len == 0) return 0;  // Empty input is invalid

    for (int i = 0; i < len; i++) {
        char c = input[i];
        // Digits 0-9: must be less than base
        if (isdigit(c)) {
            if (c - '0' >= base) {
                return 0;
            }
        }
        // Letters A-F (uppercase): must be less than base (10<=value<base)
        else if (c >= 'A' && c <= 'F') {
            if ((c - 'A' + 10) >= base) {
                return 0;
            }
        }
        // Other characters (lowercase letters, symbols, etc.) are invalid
        else {
            return 0;
        }
    }
    return 1;  // Input is valid
}

// Check if input overflows long long range (only for decimal input, other bases won't overflow naturally)
int isOverflow(const char *input) {
    const char *maxStr = "9223372036854775807";  // Maximum value of long long
    int len = strlen(input);
    int maxLen = strlen(maxStr);

    if (len > maxLen) return 1;  // Length exceeds max, overflow
    if (len < maxLen) return 0;  // Length less than max, no overflow
    if (strcmp(input, maxStr) > 0) return 1;  // Same length but larger value, overflow
    return 0;  // No overflow
}

// Any base ¡ú Decimal (intermediate state): return long long, support 2-16 bases
long long strToDecimal(const char *input, int base) {
    long long result = 0;
    int len = strlen(input);

    for (int i = 0; i < len; i++) {
        char c = input[i];
        int val;
        if (isdigit(c)) {
            val = c - '0';
        } else {
            val = c - 'A' + 10;
        }
        result = result * base + val;  // Iteratively calculate decimal value
    }
    return result;
}

// Decimal ¡ú Any base: store result in string (support 2-16 bases)
void decimalToBase(long long num, int base, char *result) {
    if (num == 0) {  // Special handling for value 0
        result[0] = '0';
        result[1] = '\0';
        return;
    }

    int idx = 0;
    // Division-remainder method: store remainders in reverse order
    while (num != 0) {
        int remainder = num % base;
        result[idx++] = MAP[remainder];
        num = num / base;
    }
    result[idx] = '\0';

    // Reverse string to get correct result
    int len = strlen(result);
    for (int i = 0; i < len / 2; i++) {
        char temp = result[i];
        result[i] = result[len - 1 - i];
        result[len - 1 - i] = temp;
    }
}

// Step visualization for Decimal¡úTarget Base
void showDecimalToBaseSteps(long long num, int base) {
    printf("?? Conversion Steps (Decimal %lld ¡ú %d-base):\n", num, base);
    if (num == 0) {
        printf("1. 0 ¡Â %d = 0 Remainder 0\n", base);
        return;
    }

    long long temp = num;
    int remainder[64], count = 0;
    // Record division-remainder process
    while (temp != 0) {
        remainder[count] = temp % base;
        printf("%d. %lld ¡Â %d = %lld Remainder %d (Corresponding char: %c)\n",
               count + 1, temp, base, temp / base, remainder[count], MAP[remainder[count]]);
        temp = temp / base;
        count++;
    }

    // Show reverse remainder process
    printf("%d. Reverse remainders ¡ú ", count + 1);
    for (int i = count - 1; i >= 0; i--) {
        printf("%c", MAP[remainder[i]]);
    }
    printf("\n");
}

// Step visualization for Binary¡úHexadecimal (4-bit grouping method)
void showBinaryToHexSteps(const char *binary) {
    int len = strlen(binary);
    printf("?? Conversion Steps (Binary %s ¡ú Hexadecimal):\n", binary);
    printf("1. Binary grouping (4 bits per group from right, pad leading zeros if needed):\n");

    // Calculate number of leading zeros needed
    int pad = (4 - len % 4) % 4;
    for (int i = 0; i < pad; i++) {
        printf("0");
    }
    for (int i = 0; i < len; i++) {
        if ((pad + i) % 4 == 0 && i != 0) {
            printf(" ");  // Group separator
        }
        printf("%c", binary[i]);
    }
    printf("\n");

    // Calculate hexadecimal value for each group
    printf("2. Convert each group to hexadecimal:\n");
    long long decimal = strToDecimal(binary, 2);
    char hexResult[20];
    decimalToBase(decimal, 16, hexResult);

    // Show calculation process for each group
    int totalLen = pad + len;
    for (int i = 0; i < totalLen; i += 4) {
        printf("   Group");
        for (int j = i; j < i + 4; j++) {
            printf("%c", (j < pad) ? '0' : binary[j - pad]);
        }
        // Calculate decimal value of the group
        int groupVal = 0;
        for (int j = 0; j < 4; j++) {
            char c = (i + j < pad) ? '0' : binary[i + j - pad];
            groupVal = groupVal * 2 + (c - '0');
        }
        printf(" ¡ú %d ¡ú %c\n", groupVal, MAP[groupVal]);
    }
    printf("3. Merge results ¡ú %s\n", hexResult);
}

// Step visualization for Binary¡úOctal (3-bit grouping method)
void showBinaryToOctSteps(const char *binary) {
    int len = strlen(binary);
    printf("?? Conversion Steps (Binary %s ¡ú Octal):\n", binary);
    printf("1. Binary grouping (3 bits per group from right, pad leading zeros if needed):\n");

    // Calculate number of leading zeros needed
    int pad = (3 - len % 3) % 3;
    for (int i = 0; i < pad; i++) {
        printf("0");
    }
    for (int i = 0; i < len; i++) {
        if ((pad + i) % 3 == 0 && i != 0) {
            printf(" ");  // Group separator
        }
        printf("%c", binary[i]);
    }
    printf("\n");

    // Calculate octal value for each group
    printf("2. Convert each group to octal:\n");
    long long decimal = strToDecimal(binary, 2);
    char octResult[20];
    decimalToBase(decimal, 8, octResult);

    // Show calculation process for each group
    int totalLen = pad + len;
    for (int i = 0; i < totalLen; i += 3) {
        printf("   Group");
        for (int j = i; j < i + 3; j++) {
            printf("%c", (j < pad) ? '0' : binary[j - pad]);
        }
        // Calculate decimal value of the group
        int groupVal = 0;
        for (int j = 0; j < 3; j++) {
            char c = (i + j < pad) ? '0' : binary[i + j - pad];
            groupVal = groupVal * 2 + (c - '0');
        }
        printf(" ¡ú %d ¡ú %c\n", groupVal, MAP[groupVal]);
    }
    printf("3. Merge results ¡ú %s\n", octResult);
}

// Format and display conversion results (including step visualization)
void showResult(const char *input, int src_base, int dest_base) {
    char result[50];
    long long decimal = strToDecimal(input, src_base);  // First convert to decimal intermediate state

    // Decimal ¡ú Target base
    decimalToBase(decimal, dest_base, result);

    // Display core results
    printf("\n=====================================\n");
    printf("  ?? Conversion Result\n");
    printf("=====================================\n");
    printf("Source value (%d-base): ", src_base);
    // Add prefix to source value (if enabled)
    if (prefixShow) {
        if (src_base == 2) printf("0b");
        else if (src_base == 8) printf("0");
        else if (src_base == 16) printf("0x");
    }
    printf("%s\n", input);

    printf("Target value (%d-base): ", dest_base);
    // Add prefix to target value (if enabled)
    if (prefixShow) {
        if (dest_base == 2) printf("0b");
        else if (dest_base == 8) printf("0");
        else if (dest_base == 16) printf("0x");
    }
    printf("%s\n", result);

    // Add decimal validation (enhance usability)
    printf("Validation (Decimal): %lld\n", decimal);
    printf("=====================================\n");

    // Step visualization (if enabled)
    if (stepVisual) {
        printf("\n-------------------------------------\n");
        if (src_base == 10) {
            showDecimalToBaseSteps(decimal, dest_base);
        } else if (src_base == 2 && dest_base == 16) {
            showBinaryToHexSteps(input);
        } else if (src_base == 2 && dest_base == 8) {
            showBinaryToOctSteps(input);
        } else {
            // Other base combinations: show "Source¡úDecimal" then "Decimal¡úTarget" steps
            printf("?? Conversion Steps (%d-base¡úDecimal¡ú%d-base):\n", src_base, dest_base);
            printf("1. %d-base %s ¡ú Decimal: %lld (Intermediate state)\n", src_base, input, decimal);
            showDecimalToBaseSteps(decimal, dest_base);
        }
        printf("-------------------------------------\n");
    }
}

// Display settings menu (toggle step visualization, number type, prefix annotation)
void showSettings() {
    char choice[10];
    while (1) {
        printf("=====================================\n");
        printf("  ??  Conversion Settings\n");
        printf("=====================================\n");
        printf("  1. Step Visualization: %s\n", stepVisual ? "Enabled" : "Disabled");
        printf("  2. Number Type: %s\n", numType ? "Signed Integer" : "Unsigned Integer");
        printf("  3. Result Prefix Annotation: %s\n", prefixShow ? "Enabled" : "Disabled");
        printf("  4. Return to Main Menu\n");
        printf("=====================================\n");
        printf("Enter setting number (1-4): ");
        getUserInput(choice, 10);

        if (strcmp(choice, "1") == 0) {
            stepVisual = !stepVisual;
            printf("? Step Visualization toggled to: %s\n\n", stepVisual ? "Enabled" : "Disabled");
        } else if (strcmp(choice, "2") == 0) {
            numType = !numType;
            printf("? Number Type toggled to: %s\n", numType ? "Signed Integer (2's complement for negatives)" : "Unsigned Integer");
            printf("??  Note: Current version only supports unsigned integer conversion; signed 2's complement can be extended\n\n");
        } else if (strcmp(choice, "3") == 0) {
            prefixShow = !prefixShow;
            printf("? Result Prefix Annotation toggled to: %s\n\n", prefixShow ? "Enabled" : "Disabled");
        } else if (strcmp(choice, "4") == 0) {
            system("cls || clear");
            printf("?? Returning to main menu...\n\n");
            break;
        } else {
            printf("? Invalid input! Please enter an integer between 1-4\n\n");
        }
    }
}
