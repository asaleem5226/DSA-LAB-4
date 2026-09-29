#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

class BinaryNumber {
private:
    struct BitNode {
        int bit;
        BitNode* next;
        BitNode* previous;

        explicit BitNode(int value)
            : bit(value), next(nullptr), previous(nullptr) {
        }
    };

    BitNode* head;
    BitNode* tail;
    size_t size;

    void appendBit(int bit) {
        BitNode* node = new BitNode(bit);

        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            node->previous = tail;
            tail->next = node;
            tail = node;
        }

        ++size;
    }

    void clear() {
        BitNode* node = head;

        while (node != nullptr) {
            BitNode* nextNode = node->next;
            delete node;
            node = nextNode;
        }

        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    string bitsToString() const {
        string bits;

        for (const BitNode* node = head;
             node != nullptr;
             node = node->next) {
            bits += static_cast<char>('0' + node->bit);
        }

        return bits;
    }

    BinaryNumber shiftedLeft(size_t places) const {
        string shiftedBits = bitsToString();
        shiftedBits.append(places, '0');
        return BinaryNumber(shiftedBits);
    }

public:
    explicit BinaryNumber(const string& inputBits)
        : head(nullptr), tail(nullptr), size(0) {
        string groupedBits = inputBits.empty() ? "0" : inputBits;

        // Left side par zeros add karke length ko 8-bit groups mein rakho.
        while (groupedBits.size() % 8 != 0) {
            groupedBits.insert(groupedBits.begin(), '0');
        }

        for (char character : groupedBits) {
            appendBit(character - '0');
        }
    }

    // Deep copy: har bit ke liye naya node banta hai.
    BinaryNumber(const BinaryNumber& other)
        : head(nullptr), tail(nullptr), size(0) {
        for (const BitNode* node = other.head;
             node != nullptr;
             node = node->next) {
            appendBit(node->bit);
        }
    }

    BinaryNumber& operator=(const BinaryNumber& other) {
        if (this != &other) {
            clear();

            for (const BitNode* node = other.head;
                 node != nullptr;
                 node = node->next) {
                appendBit(node->bit);
            }
        }

        return *this;
    }

    ~BinaryNumber() {
        clear();
    }

    void display() const {
        size_t position = 0;

        for (const BitNode* node = head;
             node != nullptr;
             node = node->next) {
            cout << node->bit;
            ++position;

            if (position % 8 == 0 && position < size) {
                cout << ' ';
            }
        }
    }

    void onesComplement() {
        for (BitNode* node = head;
             node != nullptr;
             node = node->next) {
            node->bit = 1 - node->bit;
        }
    }

    void twosComplement() {
        // 2's complement = 1's complement + 1.
        onesComplement();

        int carry = 1;
        BitNode* node = tail;

        while (node != nullptr && carry != 0) {
            int sum = node->bit + carry;
            node->bit = sum % 2;
            carry = sum / 2;
            node = node->previous;
        }

        // Leftmost bit se aage ka carry fixed width mein discard hota hai.
    }

    static BinaryNumber add(const BinaryNumber& first,
                            const BinaryNumber& second) {
        const BitNode* left = first.tail;
        const BitNode* right = second.tail;
        int carry = 0;
        string reversedResult;

        // Addition rightmost bit se left ki taraf hoti hai.
        while (left != nullptr || right != nullptr || carry != 0) {
            int sum = carry;

            if (left != nullptr) {
                sum += left->bit;
                left = left->previous;
            }

            if (right != nullptr) {
                sum += right->bit;
                right = right->previous;
            }

            reversedResult += static_cast<char>('0' + (sum % 2));
            carry = sum / 2;
        }

        reverse(reversedResult.begin(), reversedResult.end());
        return BinaryNumber(reversedResult);
    }

    static BinaryNumber multiply(const BinaryNumber& multiplicand,
                                 const BinaryNumber& multiplier) {
        BinaryNumber result("0");
        const BitNode* multiplierBit = multiplier.tail;
        size_t shift = 0;

        // Multiplier ke har 1 bit par shifted multiplicand add hota hai.
        while (multiplierBit != nullptr) {
            if (multiplierBit->bit == 1) {
                BinaryNumber partialProduct =
                    multiplicand.shiftedLeft(shift);
                result = add(result, partialProduct);
            }

            multiplierBit = multiplierBit->previous;
            ++shift;
        }

        return result;
    }

    string toDecimalString() const {
        string decimal = "0";

        // Har bit ke liye: decimal = decimal * 2 + bit.
        for (const BitNode* node = head;
             node != nullptr;
             node = node->next) {
            int carry = node->bit;

            for (int i = static_cast<int>(decimal.size()) - 1;
                 i >= 0;
                 --i) {
                size_t index = static_cast<size_t>(i);
                int value = (decimal[index] - '0') * 2 + carry;

                decimal[index] =
                    static_cast<char>('0' + (value % 10));
                carry = value / 10;
            }

            if (carry != 0) {
                decimal.insert(
                    decimal.begin(),
                    static_cast<char>('0' + carry)
                );
            }
        }

        return decimal;
    }
};

bool readBinaryInput(const string& label, string& bits) {
    while (true) {
        cout << label;

        if (!(cin >> bits)) {
            return false;
        }

        bool valid = !bits.empty();

        for (char character : bits) {
            if (character != '0' && character != '1') {
                valid = false;
                break;
            }
        }

        if (valid) {
            return true;
        }

        cout << "Please enter only 0 and 1, with no spaces.\n";
    }
}

BinaryNumber* chooseNumber(BinaryNumber& numberA,
                           BinaryNumber& numberB) {
    int choice;

    cout << "Choose number (1 = A, 2 = B): ";

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice.\n";
        return nullptr;
    }

    if (choice == 1) {
        return &numberA;
    }

    if (choice == 2) {
        return &numberB;
    }

    cout << "Choose 1 or 2.\n";
    return nullptr;
}

int main() {
    string bitsA;
    string bitsB;

    if (!readBinaryInput("Enter binary number A: ", bitsA) ||
        !readBinaryInput("Enter binary number B: ", bitsB)) {
        cout << "Input ended unexpectedly.\n";
        return 1;
    }

    BinaryNumber numberA(bitsA);
    BinaryNumber numberB(bitsB);

    while (true) {
        cout << "\n========== Binary DLL Menu ==========\n";
        cout << "1. Display A and B\n";
        cout << "2. 1's complement (choose A or B)\n";
        cout << "3. 2's complement (choose A or B)\n";
        cout << "4. Add A + B\n";
        cout << "5. Multiply A x B\n";
        cout << "6. Convert to decimal (choose A or B)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        int choice;

        if (!(cin >> choice)) {
            cout << "Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            cout << "A = ";
            numberA.display();

            cout << "\nB = ";
            numberB.display();
            cout << '\n';
        } else if (choice == 2) {
            BinaryNumber* selected = chooseNumber(numberA, numberB);

            if (selected != nullptr) {
                selected->onesComplement();
                cout << "1's complement = ";
                selected->display();
                cout << '\n';
            }
        } else if (choice == 3) {
            BinaryNumber* selected = chooseNumber(numberA, numberB);

            if (selected != nullptr) {
                selected->twosComplement();
                cout << "2's complement = ";
                selected->display();
                cout << '\n';
            }
        } else if (choice == 4) {
            BinaryNumber result = BinaryNumber::add(numberA, numberB);
            cout << "A + B = ";
            result.display();
            cout << '\n';
        } else if (choice == 5) {
            BinaryNumber result =
                BinaryNumber::multiply(numberA, numberB);
            cout << "A x B = ";
            result.display();
            cout << '\n';
        } else if (choice == 6) {
            BinaryNumber* selected = chooseNumber(numberA, numberB);

            if (selected != nullptr) {
                cout << "Decimal = "
                     << selected->toDecimalString() << '\n';
            }
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}