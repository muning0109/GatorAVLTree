#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct Node
{
    //basic attributes of a node
    string name;
    int ID;
    int balFactor;
    Node* left;
    Node* right;
    //left children and right children are null when created
    Node(string Name, int id){name = Name; ID = id; left = nullptr; right = nullptr;};
};

class AVl
{
private:
    //it's easy to point root to anything else
    //for safety root and nodeCount are private data members
    Node* root;
    int nodeCount;
public:
    Node* getRoot(){return root;};

    void setRoot(Node* root){this->root = root;};

    int getnodeCount(){return nodeCount;};

    //when the AVL tree is constructed, root is null, and there is no nodes inside
    AVl(){root = nullptr; nodeCount = 0;};

    //this comes from the slide
    Node* rotateLeft(Node *node)
    {
        Node* grandChild = node->right->left;
        Node* newParent = node->right;
        newParent->left = node;
        node->right = grandChild;
        return newParent;
    }

    //revert the rotateLeft to get rotateRight
    Node* rotateRight(Node *node)
    {
        Node* grandChild = node->left->right;
        Node* newParent = node->left;
        newParent->right = node;
        node->left = grandChild;
        return newParent;
    }

    Node* rotateRightLeft(Node *node)
    {
        Node* greatGrandChild = node->right->left->right;
        Node* newChild = node->right->left;
        Node* grandChild = node->right;
        node->right = newChild;
        newChild->right = grandChild;
        grandChild->left = greatGrandChild;
        Node* newParent = rotateLeft(node);
        return newParent;
    }

    Node* rotateLeftRight(Node *node)
    {
        Node* greatGrandChild = node->left->right->left;
        Node* newChild = node->left->right;
        Node* grandChild = node->left;
        node->left = newChild;
        newChild->left = grandChild;
        grandChild->right = greatGrandChild;
        Node* newParent = rotateRight(node);
        return newParent;
    }

    //find height by recursively going to the bottom of the tree
    int height(Node* root)
    {
        if(root == nullptr)
            return 0;
        int left = height(root->left);
        int right = height(root->right);
        return 1 + max(left, right);
    }

    //balance factor is height of left subtree minus right subtree
    int balanceFactor(Node* root) {
        if (root == nullptr)
            return 0;
        else
            return height(root->left) - height(root->right);

    }

    Node* insert(Node* root, string name, int ID)
    {
        //the first part is BST insertion
        //insert a new node at leaf
        if (root == nullptr)
        {
            Node* node = new Node(name, ID);
            cout << "successful" << endl;
            nodeCount++;
            return node;
        }

        else if (root->ID > ID)
            root->left = insert(root->left, name, ID);
        else if (root->ID < ID)
            root->right = insert(root->right, name, ID);
            //if the node already exists, insert is not successful
        else {
            cout << "unsuccessful" << endl;
            return root;
        }

        //find first unbalanced node from bottom
        root->balFactor = balanceFactor(root);

        //perform rotations to balance the tree
        if (root->balFactor > 1 && ID < root->left->ID)
            return rotateRight(root);
        else if (root->balFactor < -1 && ID > root->right->ID)
            return rotateLeft(root);
        else if (root->balFactor > 1 && ID > root->left->ID)
            return rotateLeftRight(root);
        else if (root->balFactor < -1 && ID < root->right->ID)
            return rotateRightLeft(root);

        return root;
    }

    void inOrder(Node* root, vector<Node*> &nodes)
    {
        //push nodes into the vector following left->root->right
        if (root == nullptr)
            return;
        inOrder(root->left, nodes);
        nodes.push_back(root);
        inOrder(root->right, nodes);
    }

    void preOrder(Node* root, vector<Node*> &nodes)
    {
        //push nodes into the vector following root->left->right
        if (root == nullptr)
            return;
        nodes.push_back(root);
        preOrder(root->left, nodes);
        preOrder(root->right, nodes);
    }

    void postOrder(Node* root, vector<Node*> &nodes)
    {
        //push nodes into the vector following left->right->root
        if (root == nullptr)
            return;
        postOrder(root->left, nodes);
        postOrder(root->right, nodes);
        nodes.push_back(root);
    }

    void printInorder(Node* root)
    {
        //print the names stored in the vector
        vector<Node*> nodes;
        inOrder(root, nodes);
        for (int i = 0; i < nodes.size(); i++) {
            if (i != nodes.size() - 1)
                cout << nodes[i]->name << ", ";
            else
                //last name doesn't follow by comma
                cout << nodes[i]->name;

        }
    }

    void printPreorder(Node* root)
    {
        vector<Node*> nodes;
        preOrder(root, nodes);
        for (int i = 0; i < nodes.size(); i++) {
            if (i != nodes.size() - 1)
                cout << nodes[i]->name << ", ";
            else
                //last name doesn't follow by comma
                cout << nodes[i]->name;
        }
    }

    void printPostorder(Node* root)
    {
        vector<Node*> nodes;
        postOrder(root, nodes);
        for (int i = 0; i < nodes.size(); i++) {
            if (i != nodes.size() - 1)
                cout << nodes[i]->name << ", ";
            else
                //last name doesn't follow by comma
                cout << nodes[i]->name;

        }
    }

    void printLevelCount(Node* root) {
        //level count is the height of root
        //most nodes in the path
        int levelCount = height(root);
        cout << levelCount << endl;

    }

    bool searchID(Node* root, int ID) {
        //ID not exist
        if (root == nullptr) {
            cout << "unsuccessful" << endl;
            return false;
        }
        //find the unique ID, return true;
        if (root->ID == ID) {
            cout << root->name << endl;
            return true;
        }
        else if (ID > root->ID) {
            searchID(root->right, ID);
        }
        else if (ID < root->ID) {
            searchID(root->left, ID);
        }
    }

    bool IDExist(Node* root, int ID) {
        //check if ID exists
        if (root == nullptr) {
            return false;
        }
        if (root->ID == ID) {
            return true;
        }
        bool left = IDExist(root->left, ID);
        if (left)
            return true;
        //if not in the left subtree, then search in the right subtree
        bool right = IDExist(root->right, ID);
        return right;
    }

    bool searchName(Node* root, string name) {
        vector<Node*> nameMatch;
        vector<Node*> nodes;
        preOrder(root, nodes);
        //get a vector of nodes stored in preOrder way

        for (int i = 0; i < nodes.size(); i++)
        {
            //if name matches, push to corresponding vector
            if (nodes[i]->name == name)
            {
                nameMatch.push_back(nodes[i]);
                if(nodes[i]->ID < 10000000)
                {
                    //fill the left digits to be 0 when ID has leading zeros
                    cout << setw(8) << setfill('0') << nodes[i]->ID << endl;
                }
                else{
                    cout << nodes[i]->ID << endl;
                }
            }

        }
        //if there is no matching name in the AVL tree, print unsuccessful
        if(nameMatch.empty())
        {
            cout << "unsuccessful" << endl;
            return false;
        }
        else
            return true;

    }

    Node* smallestNode(Node* node)
    {
        //helps find the inorder successor
        Node* temp = node;
        while(temp->left != nullptr)
            temp = temp->left;
        return temp;
    }

    Node* remove(Node* root, int ID)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        if (ID > root->ID)
            root->right = remove(root->right, ID);
        else if (ID < root->ID)
            root->left = remove(root->left, ID);
        else {
            nodeCount--;
            //when there is no children
            if (root->left == nullptr && root->right == nullptr)
            {
                delete root;
                root = nullptr;
                return root;
            }
                //only has right children
            else if(root->left == nullptr)
            {
                Node* temp = root;
                root = root->right;
                delete temp;
                return root;
            }
                //only has left children
            else if(root->right == nullptr)
            {
                Node* temp = root;
                root = root->left;
                delete temp;
                return root;
            }
            else
                //have two children
                //copy the inorder successor's value to the root
                //remove the duplicate from right subtree
            {
                Node* temp = smallestNode(root->right);
                root->ID = temp->ID;
                root->name = temp->name;
                root->right = remove(root->right, root->ID);
                return root;
            }
        }
        return root;
    }

    Node* removeInorder(Node* root, int count)
    {
        //if the tree is empty
        if(root == nullptr)
        {
            cout << "unsuccessful" << endl;
            return root;
        }

        vector<Node*> nodes;
        inOrder(this->root, nodes);
        //when count is out of range
        if( count > nodes.size()-1)
        {
            cout << "unsuccessful" << endl;
            return root;
        }
        else{
            //call the remove function to remove the targeted node
            Node* temp = remove(this->root, nodes[count]->ID);
            cout << "successful" << endl;
            return temp;
        }
    }
};

bool checkID(string ID)
{
    //the ID should have a length 8
    //the ID should only have digits
    if (ID.length() != 8)
        return false;
    bool valid = true;
    for(int i = 0; i < ID.length(); i++)
    {
        if (isdigit(ID.at(i)))
            valid = true;
        else
        {
            valid = false;
            break;
        }
    }
    return valid;
}

bool checkNAME (string name) {
    //name must only have letters and spaces
    //the string is case sensitive
    bool valid = true;
    for(int i = 0; i < name.length(); i++)
    {
        if (isalpha(name[i]) || isspace(name[i]))
            valid = true;
        else
        {
            valid = false;
            break;
        }

    }

    return valid;
}

bool checkOrder (string n)
{
    //n must be composed of digits
    //check whether n is in removeInorder
    bool valid = true;
    for(int i = 0; i < n.length(); i++)
    {
        if (isdigit(n.at(i)))
            valid = true;
        else
        {
            valid = false;
            break;
        }
    }
    return valid;
}

void callingFunction(string command, AVl &gatorAVL)
{
    string name;
    string check;
    int ID;
    int index = command.find_first_of(' ');
    //getting the function we are supposed to call
    string function = command.substr(0, index);
    command = command.substr(index + 1);
    if (function == "insert") {
        command = command.substr(1);
        index = command.find_first_of('\"');
        //get the name for insertion by slicing string
        name = command.substr(0, index);
        //check is the gatorID
        //need to verify its validity before converting to integer
        check = command.substr(index + 2);
        //three conditions to initiating insert
        //1. the ID is valid
        //2. the name is valid
        if (checkID(check) && checkNAME(name)) {
            ID = stoi(check);
            Node* newRoot = gatorAVL.insert(gatorAVL.getRoot(), name, ID);
            gatorAVL.setRoot(newRoot);
        }
        else
            //if it fails any conditions, the insertion is unsuccessful
            cout << "unsuccessful" << endl;
    }
    else if (function == "remove") {
        //when remove ID, the ID must be valid
        if(checkID(command))
        {
            ID = stoi(command);
            //only trying remove when the ID exists in the tree
            if(gatorAVL.IDExist(gatorAVL.getRoot(), ID))
            {
                cout << "successful" << endl;
                gatorAVL.setRoot(gatorAVL.remove(gatorAVL.getRoot(), ID));
            }
            else
                cout << "unsuccessful" << endl;

        } else
            cout << "unsuccessful" << endl;

    }
    else if (function == "search") {
        //identify search by ID or search by name
        if (command[0] == '"') {
            name = command.substr(1, command.length() - 2);
            //the name needs to be valid
            if(checkNAME(name))
                gatorAVL.searchName(gatorAVL.getRoot(), name);
            else
                cout << "unsuccessful" << endl;

        }
        else {
            //the ID needs to be valid
            if (checkID(command))
            {
                ID = stoi(command);
                gatorAVL.searchID(gatorAVL.getRoot(), ID);
            }
            else
                cout << "unsuccessful" << endl;
        }
    }
    else if (function == "printInorder") {
        gatorAVL.printInorder(gatorAVL.getRoot());
        cout << endl;
    }
    else if (function == "printPreorder") {
        gatorAVL.printPreorder(gatorAVL.getRoot());
        cout << endl;
    }
    else if (function == "printPostorder") {
        gatorAVL.printPostorder(gatorAVL.getRoot());
        cout << endl;
    }
    else if (function == "printLevelCount")
        gatorAVL.printLevelCount(gatorAVL.getRoot());
    else if (function == "removeInorder") {
        //the Nth should be a valid number
        if(checkOrder(command))
        {
            int n = stoi(command);
            gatorAVL.setRoot(gatorAVL.removeInorder(gatorAVL.getRoot(), n));
        }
        else
            cout << "unsuccessful" << endl;
    }
}


int main() {
    AVl gatorAVL;
    int input;
    cin >> input;
    string line;
    vector<string> commands;

    //read each line and store them into a vector
    for (int i = 0; i < input + 1; i++)
    {
        getline(cin, line);
        commands.push_back(line);
    }

    //execute commands on each line
    for(int i = 0; i < commands.size(); i++)
    {
        string command = commands[i];
        callingFunction(command, gatorAVL);
    }
    return 0;
}