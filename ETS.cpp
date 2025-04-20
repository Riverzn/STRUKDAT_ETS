#include <iostream>
#include <string>

using namespace std;

struct TextNode {
    char ch;
    bool bold;
    bool italic;
    bool underline;
    TextNode* next;

    TextNode(char c) : ch(c), bold(false), italic(false), underline(false), next(nullptr) {}
};

struct Action {
    string type; // "insert", "delete", "format"
    int position;
    string content;
    bool bold = false;
    bool italic = false;
    bool underline = false;
};

class Stack {
private:
    Action* act;
    int top;
    int capacity;
public:
    Stack() {
        capacity = 100;
        top = -1;
        act = new Action[capacity];
    }
    ~Stack() {
        delete[] act;
    }
    bool isFull() {
        return top == capacity - 1;
    }
    bool isEmpty() {
        return top == -1;
    }
    Action peek() {
        if (!isEmpty()) return act[top];
        throw runtime_error("Stack is empty");
    }
    void push(Action data) {
        if (isFull()) {
            cout << "Stack overflow!" << endl;
            return;
        }
        act[++top] = data;
    }
    void pop() {
        if (isEmpty()) {
            cout << "Stack underflow!" << endl;
            return;
        }
        top--;
    }
};

void printText(TextNode* head) {
    TextNode* temp = head;
    while (temp) {
        if (temp->bold) cout << "\033[1m";
        if (temp->italic) cout << "\033[3m";
        if (temp->underline) cout << "\033[4m";

        cout << temp->ch;

        cout << "\033[0m";
        temp = temp->next;
    }
    cout << endl;
}

int main() {
    TextNode* head = nullptr;
    Stack undoStack;
    Stack redoStack;

    bool boldMode = false;
    bool italicMode = false;
    bool underlineMode = false;

    int choice;

    while (true) {
    cout << "\n";
    cout << "╔══════════════════════════════════════╗\n";
    cout << "║         📝  TEXT EDITOR CLI          ║\n";
    cout << "╠══════════════════════════════════════╣\n";
    cout << "║  1️⃣  ✍️  Insert Text                  ║\n";
    cout << "║  2️⃣  ❌  Delete Text                  ║\n";
    cout << "║  3️⃣  🎨  Format Specific Text         ║\n";
    cout << "║  4️⃣  🔀  Toggle Format Mode (B/I/U)   ║\n";
    cout << "║  5️⃣  ↩️  Undo Last Action             ║\n";
    cout << "║  6️⃣  ↪️  Redo Last Undone Action      ║\n";
    cout << "║  7️⃣  📄  Display Current Text         ║\n";
    cout << "║  0️⃣  🚪  Exit Editor                  ║\n";
    cout << "╚══════════════════════════════════════╝\n";
    cout << "👉 Enter your choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 0:
        cout << "\n👋 Exiting the editor. See you again!\n";
        return 0;
        
        case 1: {
            cout << "Insert Choice:\n";
            cout << "1. Insert at end\n";
            cout << "2. Insert at beginning\n";
            cout << "3. Insert at specific position\n";
            int insertChoice;
            cin >> insertChoice;
            cin.ignore();

            string input, line;
            cout << "Enter text to insert (type '::end' to finish):" << endl;
            while (true) {
                getline(cin, line);
                if (line == "::end") break;
                input += line + '\n';
            }
            switch (insertChoice) {
                case 1: { // Insert at end
                    int position = 0;
                    TextNode* temp = head;
                    while (temp) {
                        position++;
                        temp = temp->next;
                    }
        
                    for (char ch : input) {
                        TextNode* newNode = new TextNode(ch);
        
                        if (boldMode) newNode->bold = true;
                        if (italicMode) newNode->italic = true;
                        if (underlineMode) newNode->underline = true;
        
                        if (!head) {
                            head = newNode;
                        } else {
                            TextNode* curr = head;
                            while (curr->next) curr = curr->next;
                            curr->next = newNode;
                        }
        
                        Action insertAction;
                        insertAction.type = "insert";
                        insertAction.position = position++;
                        insertAction.content = ch;
                        insertAction.bold = newNode->bold;
                        insertAction.italic = newNode->italic;
                        insertAction.underline = newNode->underline;
                        undoStack.push(insertAction);
                    }
        
                    while (!redoStack.isEmpty()) redoStack.pop();
        
                    cout << "Inserted at end: \"" << input << "\"" << endl;
                    break;
                }
            
                    case 2: { // Insert at beginning
                        int position = 0;
                        TextNode* newHead = nullptr;
                        TextNode* newTail = nullptr;
            
                        for (char ch : input) {
                            TextNode* newNode = new TextNode(ch);
            
                            if (boldMode) newNode->bold = true;
                            if (italicMode) newNode->italic = true;
                            if (underlineMode) newNode->underline = true;
            
                            if (!newHead) {
                                newHead = newNode;
                                newTail = newNode;
                            } else {
                                newTail->next = newNode;
                                newTail = newNode;
                            }
            
                            Action insertAction;
                            insertAction.type = "insert";
                            insertAction.position = position;
                            insertAction.content = ch;
                            insertAction.bold = newNode->bold;
                            insertAction.italic = newNode->italic;
                            insertAction.underline = newNode->underline;
                            undoStack.push(insertAction);
                        }
    
                        newTail->next = head;
                        head = newHead;
            
                        while (!redoStack.isEmpty()) redoStack.pop();
            
                        cout << "Inserted at beginning: \"" << input << "\"" << endl;
                        break;
                    }
            
                    case 3: { // Insert at specific position
                        int pos;
                        cout << "Enter position to insert at: ";
                        cin >> pos;
                        cin.ignore();
            
                        int position = 0;
                        TextNode* curr = head;
                        TextNode* prev = nullptr;
            
                        while (curr && position < pos) {
                            prev = curr;
                            curr = curr->next;
                            position++;
                        }
            
                        if (position != pos) {
                            cout << "Invalid position!" << endl;
                            break;
                        }
            
                        for (char ch : input) {
                            TextNode* newNode = new TextNode(ch);
            
                            if (boldMode) newNode->bold = true;
                            if (italicMode) newNode->italic = true;
                            if (underlineMode) newNode->underline = true;
            
                            if (prev) {
                                prev->next = newNode;
                            } else {
                                head = newNode;
                            }
            
                            newNode->next = curr;
                            prev = newNode;
            
                            Action insertAction;
                            insertAction.type = "insert";
                            insertAction.position = position++;
                            insertAction.content = ch;
                            insertAction.bold = newNode->bold;
                            insertAction.italic = newNode->italic;
                            insertAction.underline = newNode->underline;
                            undoStack.push(insertAction);
                        }
            
                        while (!redoStack.isEmpty()) redoStack.pop();
            
                        cout << "Inserted at position " << pos << ": \"" << input << "\"" << endl;
                        break;
                    }
            
                    default:
                        cout << "Invalid insert choice.\n";
                }
                break;
        }


        case 2: {
            cout << "Delete Choice:\n";
            cout << "1. Delete at end\n";
            cout << "2. Delete at beginning\n";
            cout << "3. Delete from specific position\n";
            int deleteChoice;
            cin >> deleteChoice;
        
            switch (deleteChoice) {
                case 1: {
                    int count;
                    cout << "Enter number of characters to delete: ";
                    cin >> count;
        
                    int length = 0;
                    TextNode* curr = head;
                    while (curr) {
                        length++;
                        curr = curr->next;
                    }
        
                    if (count <= 0 || count > length) {
                        cout << "Invalid delete count.\n";
                        break;
                    }
        
                    int deletePos = length - count;
                    curr = head;
                    TextNode* prev = nullptr;
                    for (int i = 0; i < deletePos; ++i) {
                        prev = curr;
                        curr = curr->next;
                    }
        
                    while (!redoStack.isEmpty()) redoStack.pop(); // Clear redo stack
        
                    for (int i = 0; i < count && curr; ++i) {
                        Action deleteAction;
                        deleteAction.type = "delete";
                        deleteAction.position = deletePos;
                        deleteAction.content = string(1, curr->ch);
                        undoStack.push(deleteAction);

                        TextNode* toDelete = curr;
                        curr = curr->next;
                        delete toDelete;
                    }
        
                    if (prev) prev->next = curr;
                    else head = curr;
        
                    cout << "Deleted: " << count << endl;
                    break;
                }
        
                case 2: {
                    int count;
                    cout << "Enter number of characters to delete: ";
                    cin >> count;
        
                    if (count <= 0) {
                        cout << "Invalid delete count.\n";
                        break;
                    }
        
                    TextNode* curr = head;
        
                    while (!redoStack.isEmpty()) redoStack.pop(); // Clear redo stack
        
                    for (int i = 0; i < count && curr; ++i) {
                        Action deleteAction;
                        deleteAction.type = "delete";
                        deleteAction.position = i;
                        deleteAction.content = string(1, curr->ch);
                        undoStack.push(deleteAction);

                        TextNode* toDelete = curr;
                        curr = curr->next;
                        delete toDelete;
                    }
        
                    head = curr;
        
                    cout << "Deleted: " << count << endl;
                    break;
                }
        
                case 3: {
                    int pos, count;
                    cout << "Enter position to delete from: ";
                    cin >> pos;
                    cout << "Enter number of characters to delete: ";
                    cin >> count;
        
                    TextNode* curr = head;
                    TextNode* prev = nullptr;
                    int index = 0;
        
                    while (curr && index < pos) {
                        prev = curr;
                        curr = curr->next;
                        index++;
                    }
        
                    if (!curr) {
                        cout << "Invalid position!" << endl;
                        break;
                    }
        
                    while (!redoStack.isEmpty()) redoStack.pop();
        
                    for (int i = 0; i < count && curr; ++i) {
                        Action deleteAction;
                        deleteAction.type = "delete";
                        deleteAction.position = pos + i;
                        deleteAction.content = string(1, curr->ch);
                        undoStack.push(deleteAction);
        
                        TextNode* toDelete = curr;
                        curr = curr->next;
                        delete toDelete;
                    }
        
                    if (prev) prev->next = curr;
                    else head = curr;
        
                    cout << "Deleted " << count << " characters from position " << pos << ".\n";
                    break;
                }
            }
            break;
        }

        case 3: {
            int pos;
            cout << "Enter position to format: ";
            cin >> pos;

            char formatType;
            cout << "Choose format (b = bold, i = italic, u = underline): ";
            cin >> formatType;

            TextNode* temp = head;
            int current = 0;
            while (temp && current < pos) {
                temp = temp->next;
                current++;
            }

            if (!temp) {
                cout << "Invalid position!" << endl;
                break;
            }

            Action formatAction;
            formatAction.type = "format";
            formatAction.position = pos;
            formatAction.bold = temp->bold;
            formatAction.italic = temp->italic;
            formatAction.underline = temp->underline;

            if (formatType == 'b') temp->bold = !temp->bold;
            else if (formatType == 'i') temp->italic = !temp->italic;
            else if (formatType == 'u') temp->underline = !temp->underline;
            else {
                cout << "Invalid format type!" << endl;
                break;
            }

            undoStack.push(formatAction);
            while (!redoStack.isEmpty()) redoStack.pop();
            cout << "Formatting applied at position " << pos << "." << endl;
            break;
        }

        case 4: {
            cout << "Toggle formatting mode (b = bold, i = italic, u = underline): ";
            char toggle;
            cin >> toggle;
        
            // Toggle individual formats
            if (toggle == 'b') {
                boldMode = !boldMode;
                cout << "Bold formatting " << (boldMode ? "enabled" : "disabled") << "." << endl;
            } 
            else if (toggle == 'i') {
                italicMode = !italicMode;
                cout << "Italic formatting " << (italicMode ? "enabled" : "disabled") << "." << endl;
            } 
            else if (toggle == 'u') {
                underlineMode = !underlineMode;
                cout << "Underline formatting " << (underlineMode ? "enabled" : "disabled") << "." << endl;
            }
            else {
                cout << "Invalid format toggle." << endl;
            }
            break;
        }

        // Undo action handling (case 5)
        case 5: {
            if (undoStack.isEmpty()) {
                cout << "Nothing to undo!" << endl;
                break;
            }

            Action lastAction = undoStack.peek();
            undoStack.pop();

            if (lastAction.type == "format") {
                TextNode* temp = head;
                int current = 0;
                while (temp && current < lastAction.position) {
                    temp = temp->next;
                    current++;
                }

                if (temp) {
                    // Restore the previous formatting state
                    temp->bold = lastAction.bold;
                    temp->italic = lastAction.italic;
                    temp->underline = lastAction.underline;
                }

                cout << "Undone formatting at position " << lastAction.position << endl;
                redoStack.push(lastAction); // push the action to redoStack for redo
            }

            if (lastAction.type == "insert") {
                // Undo insertion by deleting the inserted characters
                int pos = lastAction.position;
                int len = lastAction.content.length();
            
                TextNode* curr = head;
                TextNode* prev = nullptr;
                int index = 0;
            
                while (curr && index < pos) {
                    prev = curr;
                    curr = curr->next;
                    index++;
                }
            
                for (int i = 0; i < len && curr; ++i) {
                    TextNode* toDelete = curr;
                    curr = curr->next;
                    delete toDelete;
                }
            
                if (prev) prev->next = curr;
                else head = curr;
            
                cout << "Undone insertion: " << lastAction.content << endl;
                redoStack.push(lastAction);
            }
            else if (lastAction.type == "delete") {
                TextNode* curr = head;
                TextNode* prev = nullptr;
                int pos = lastAction.position;
                int index = 0;

                // Traverse to the deletion position
                while (curr && index < pos) {
                    prev = curr;
                    curr = curr->next;
                    index++;
                }

                // Re-insert the deleted content at the correct position
                TextNode* restoredHead = nullptr;
                TextNode* restoredTail = nullptr;
                for (char ch : lastAction.content) {
                    TextNode* newNode = new TextNode(ch);
                    newNode->bold = lastAction.bold;       // Restore formatting
                    newNode->italic = lastAction.italic;   // Restore formatting
                    newNode->underline = lastAction.underline; // Restore formatting

                    if (!restoredHead) {
                        restoredHead = newNode;
                        restoredTail = newNode;
                    } else {
                        restoredTail->next = newNode;
                        restoredTail = newNode;
                    }
                }

                // Insert the restored content
                if (prev) {
                    prev->next = restoredHead;
                } else {
                    head = restoredHead;
                }

                // Link the rest of the list after the restored content
                restoredTail->next = curr;

                cout << "Undone deletion: " << lastAction.content << endl;

                // Push the action to the redo stack
                redoStack.push(lastAction);
            }

            break;
        }

        // Redo action handling (case 6)
        case 6: {
            if (redoStack.isEmpty()) {
                cout << "Nothing to redo!" << endl;
                break;
            }

            Action redoAction = redoStack.peek();
            redoStack.pop();

            if (redoAction.type == "format") {
                TextNode* temp = head;
                int current = 0;
                while (temp && current < redoAction.position) {
                    temp = temp->next;
                    current++;
                }

                if (temp) {
                    // Reapply the formatting
                    temp->bold = redoAction.bold;
                    temp->italic = redoAction.italic;
                    temp->underline = redoAction.underline;
                }

                cout << "Redone formatting at position " << redoAction.position << endl;
                undoStack.push(redoAction); // push the action to undoStack for undo
            }

            if (redoAction.type == "insert") {
                int pos = redoAction.position;
                string content = redoAction.content;

                // Create new nodes for the content being redone
                TextNode* newHead = nullptr;
                TextNode* newTail = nullptr;

                for (char ch : content) {
                    TextNode* newNode = new TextNode(ch);
                    newNode->bold = redoAction.bold;       // Apply formatting
                    newNode->italic = redoAction.italic;   // Apply formatting
                    newNode->underline = redoAction.underline; // Apply formatting

                    if (!newHead) {
                        newHead = newNode;
                        newTail = newNode;
                    } else {
                        newTail->next = newNode;
                        newTail = newNode;
                    }
                }

                // Find the position where we need to insert
                TextNode* curr = head;
                TextNode* prev = nullptr;
                int index = 0;
                while (curr && index < pos) {
                    prev = curr;
                    curr = curr->next;
                    index++;
                }

                if (prev) {
                    prev->next = newHead;
                } else {
                    head = newHead; // If inserting at the head, update the head
                }

                newTail->next = curr; // Attach the rest of the list

                cout << "Redone insert: " << content << endl;

                // Push the action to the undo stack for potential future undo
                undoStack.push(redoAction);
            }

            else if (redoAction.type == "delete") {
                // Redo deletion
                string content = redoAction.content;
                int pos = redoAction.position;

                TextNode* curr = head;
                TextNode* prev = nullptr;
                int index = 0;

                // Traverse to the position where we need to delete
                while (curr && index < pos) {
                    prev = curr;
                    curr = curr->next;
                    index++;
                }

                // Delete the content from the list
                string deletedText = "";
                for (int i = 0; i < content.length() && curr; ++i) {
                    deletedText += curr->ch;
                    TextNode* toDelete = curr;
                    curr = curr->next;
                    delete toDelete;
                }

                if (prev) {
                    prev->next = curr;
                } else {
                    head = curr; // If deleting from the head, update the head
                }

                cout << "Redone delete: " << deletedText << endl;

                // Push the action to the undo stack for potential future undo
                undoStack.push(redoAction);
            }

            break;
        }

        case 7: {
            cout << "Current Text: \n";
            printText(head);
            break;
        }

        default:
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
