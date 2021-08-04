#include <iostream>
#include <map>
#include <set>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
using namespace std;

// get file' s size
int file_size(char* filename) {
    int size;
    FILE *fp = fopen(filename, "r");
    if(!fp)
        return -1;
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fclose(fp);
    return size;
}
// get file' s content
string get_text(char *file_name) {
    char *buffer;
    string res;
    int size = file_size(file_name);
    FILE *fp = fopen(file_name, "r");
    if(!fp)
        return NULL;
    buffer = (char *)malloc((size + 10) * sizeof(char));
    while (fgets(buffer, size + 10, fp))
        res += string(buffer);
    fclose(fp);
    return res;
}
// get the number of chars
int get_size(string &text) {
    set<char> charset;
    // get all the appeared char
    for (string::iterator it = text.begin(); it != text.end(); it++)
        charset.insert(*it);
    return charset.size();
}
// get chars, count weights and return size
void get_char_weight(string &text, char *char_list, double *weights) {
    int ctr = 0;
    map<char, double> char2frequency;
    set<char> charset;
    // get all the appeared char
    for (string::iterator it = text.begin(); it != text.end(); it++)
        charset.insert(*it);
    // initialize frequencies of all chars
    for (set<char>::iterator it = charset.begin(); it != charset.end(); it++)
        char2frequency[*it] = 0;
    // count frequencies
    for (string::iterator it = text.begin(); it != text.end(); it++)
        char2frequency[*it] += 1;
    // count probabilities
    for (set<char>::iterator it = charset.begin(); it != charset.end(); it++)
        char2frequency[*it] /= text.length();
    // put chars and probabilities to the two list
    for (set<char>::iterator it = charset.begin(); it != charset.end(); it++, ctr++) {
        weights[ctr] = char2frequency[*it];
        char_list[ctr] = *it;
    }
}

// Node class
struct Node {
    bool coded;
    char data;
    double weight;
    Node *parent, *lchild, *rchild;
    Node() {
        data = '\0';
        coded = false;
        parent = lchild = rchild = NULL;
    }
};
// Huffman tree class
class HuffmanTree {
    int num;
    Node *root, *nodes;
    map<char, string> data2code;
public:
    HuffmanTree(char *info, double *weights, int n) {
        num = n;
        nodes = new Node[2 * num - 1];
        for (int i = 0; i < num; i++) {
            nodes[i].data = info[i];
            nodes[i].weight = weights[i];
        }
        _initCode();
    }
    ~HuffmanTree() {
        delete[] nodes;
    }
    void show() {
        for (int i = 0; i < num; i++)
            cout << nodes[i].data << ": " << data2code[nodes[i].data] << endl;
    }
    string encode(string plaintext) {
        string ciphertext;
        for (string::iterator it = plaintext.begin(); it != plaintext.end(); it++)
            ciphertext += data2code[*it];
        return ciphertext;
    }
    string decode(string ciphertext) {
        string plaintext;
        Node *node;
        for (string::iterator it = ciphertext.begin(); it != ciphertext.end(); it++) {
            for (node = root; !node->data && it != ciphertext.end(); it++) {
                if (*it == '0') {
                    node = node->lchild;
                } else if (*it == '1') {
                    node = node->rchild;
                } else {
                    return "error";
                }
            }
            if (!node->data && it == ciphertext.end()) {
                return "error";
            } else {
                plaintext += node->data;
                it--;
            }
        }
        return plaintext;
    }
private:
    void _initCode() {
        for (int i = 0; i < num - 1; i++) {
            int min1, min2;
            for (int j = 0; j < num + i; j++)
                if (!nodes[j].coded) {
                    min1 = j;
                    break;
                }
            for (int j = 0; j < num + i; j++)
                if (!nodes[j].coded && nodes[j].weight < nodes[min1].weight)
                    min1 = j;
            for (int j = 0; j < num + i; j++)
                if (!nodes[j].coded && j != min1) {
                    min2 = j;
                    break;
                }
            for (int j = 0; j < num + i; j++)
                if (!nodes[j].coded && j != min1 && nodes[j].weight < nodes[min2].weight)
                    min2 = j;
            nodes[num + i].weight = nodes[min1].weight + nodes[min2].weight;
            nodes[num + i].lchild = nodes + min1;
            nodes[num + i].rchild = nodes + min2;
            nodes[min1].parent = nodes[min2].parent = nodes + num + i;
            nodes[min1].coded = nodes[min2].coded = true;
        }
        root = nodes + 2 * num - 2;
        Node *node;
        for (int i = 0; i < num; i++) {
            data2code[nodes[i].data] = "";
            for (node = nodes + i; node->parent; node = node->parent) {
                if (node->parent->lchild == node)
                    data2code[nodes[i].data] = "0" + data2code[nodes[i].data];
                else
                    data2code[nodes[i].data] = "1" + data2code[nodes[i].data];
            }
        }
    }
};

int main() {
    char *char_list;
    char plaintext_file_path[] = "./plaintext.txt";
    char ciphertext_file_path[] = "./ciphertext.txt";
    int size;
    double *weights;

    // output the original text
    string text = get_text(plaintext_file_path);
    cout << text << endl;

    // get chars and weights
    size = get_size(text);
    weights = new double[size];
    char_list = new char[size];
    get_char_weight(text, char_list, weights);

    // create Huffman tree
    HuffmanTree huffman_tree(char_list, weights, size);

    // get ciphertext code
    string code = huffman_tree.encode(text);
    // open ciphertext file
    FILE *ciphertext_file = fopen(ciphertext_file_path, "w+");
    // write ciphertext to this file
    fprintf(ciphertext_file, "%s", code.c_str());
    // close this file
    fclose(ciphertext_file);

    // re-open this file with 'read' mode
    ciphertext_file = fopen(ciphertext_file_path, "r+");
    // get ciphertext file' s text, which is a code including 0 and 1
    string ciphertext = get_text(ciphertext_file_path);
    // output ciphertext
    cout << ciphertext << endl;

    // get plaintext by decoding the ciphertext code
    string plaintext = huffman_tree.decode(ciphertext);
    // output plaintext
    cout << plaintext << endl;
    // close ciphertext file
    fclose(ciphertext_file);

    // free memories
    delete[] weights;
    delete[] char_list;

    return 0;
}
