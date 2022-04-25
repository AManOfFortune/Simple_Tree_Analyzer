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
        std::cout << n->key << "; ";
        printTree(n->right);
    }
}

void insertElement(struct node* root, int key){
    if (root == nullptr) {
        root = new struct node;
        root->key = key;
        root->left = nullptr;
        root->right = nullptr;

    } else {
        struct node* current = root;
        if (key > current->key){
            insertElement(current->left, key);
        }
        else{
            insertElement(current->right, key);
        }
    }
    printTree(root);
};

void importData(std::string fileName){
    std::ifstream Inputfile (fileName);
    std::string helpString;
    int helpVar;
    struct node* root = nullptr;

    if(Inputfile.is_open()){
        while (getline(Inputfile, helpString)) {
            //Insert new nodes into BST
            //stoi casts String to int
            helpVar = stoi(helpString);
            insertElement(root, helpVar);
        }
        Inputfile.close();
    }
    else{
        std::cout << "Unable to open file" << std::endl;
    }
    printTree(root);
}

int main()
{
    std::string fileName = "";
//min, max and avg are just currently placeholders
    auto avgPtr = std::make_shared<int>(10);

    //Flag to stop looping if user wants to exit
    bool loop = true;
    while (loop) {
        system("cls");
        std::cout << "Insert filename: " << std::endl;
        std::cin >> fileName;
        if(fileName != ""){
            break;
        }
    }
    importData(fileName);
//    std::cout << "min:" << minimum << ", max: " << maximum << ", avg: " << avg << std::endl;
    return 0;
}
