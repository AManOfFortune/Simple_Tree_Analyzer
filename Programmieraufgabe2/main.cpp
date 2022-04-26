#include <iostream>
#include <fstream>
#include <string>
#include <memory>

struct node
{
    int key;
    struct node* left;
    struct node* right;
    //struct node* parent;
};

//prints BST inorder
void printTree(struct node* n) {
    if (n != nullptr) {
        printTree(n->left);
        std::cout << n->key << std::endl;
        printTree(n->right);
    }
}

struct node* insertElement(struct node* n, int key) {
    if (n == nullptr) {
        n = new struct node;
        n->key = key;
        n->left = nullptr;
        n->right = nullptr;
    }
    else {
        if (key > n->key) {
            n->left = insertElement(n->left, key);
        }
        else {
            n->right = insertElement(n->right, key);
        }
    }
    return n;
};

struct node* importData(std::string fileName, struct node* root) {
    std::ifstream Inputfile(fileName);
    std::string line;

    if (Inputfile.is_open()) {
        while (getline(Inputfile, line)) {
            //Insert new nodes into BST
            //stoi casts String to int
            root = insertElement(root, stoi(line));
        }
        Inputfile.close();
    }
    else {
        std::cout << "Unable to open file" << std::endl;
    }

    return root;
}

int main()
{
    std::string fileName = "Inputfile.txt";
    struct node* root = nullptr;
    //min, max and avg are just currently placeholders
    auto avgPtr = std::make_shared<int>(10);

    while (fileName == "") {
        system("cls");
        std::cout << "Insert filename: " << std::endl;
        std::cin >> fileName;
    }

    root = importData(fileName, root);
    printTree(root);
    //    std::cout << "min:" << minimum << ", max: " << maximum << ", avg: " << avg << std::endl;
    return 0;
}