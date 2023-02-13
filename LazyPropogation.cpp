#include <iostream>
#include <algorithm>
using namespace std;
typedef int type;

static const int maxLen = 3000;
int len;
type seg[2 * maxLen + 3];
type lazy[2 * maxLen + 3];
type arr[maxLen];
const type defaultLazyValue = 0;

int leftChild(int parent) {
    return (2 * parent) + 1;
}
int rightChild(int parent) {
    return (2 * parent) + 2;
}
int parent(int child) {
    return (child - 1) / 2;
}

type identityElement = INT_MAX;
//how to calculate parent from its children
type calculateParent(type child1, type child2) {
    return min(child1, child2);
}

//when lazyValue nonzero found update segmenttree before pushing it
void updateTreeWithLazyVal(type& segVal, type lazyVal, int mostLeft, int mostRight) {
    segVal += lazyVal;
}

//how to change lazy values
void updateLazyValue(type& lazyVal, type addVal) {
    lazyVal += addVal;
}





void getInput() {

    cin >> len;

    if (len > maxLen) {
        throw std::invalid_argument("Maximum length exceeded!!");
    }

    for (int i = 0; i < len; i++)
        cin >> arr[i];

}

void build(int node, int l, int r) {

    if (l == r) {
        seg[node] = arr[l];
        return;
    }

    build(leftChild(node), l, (l + r) / 2);
    build(rightChild(node), 1 + ((l + r) / 2), r);

    seg[node] = calculateParent(seg[leftChild(node)], seg[rightChild(node)]);
}

void init() {
    build(0, 0, len - 1);

    if (defaultLazyValue != 0) {
        for (int i = 0; i < 2 * len + 3; i++)
            lazy[i] = defaultLazyValue;
    }

}


type find(int node, int findLeft, int findRight, int mostLeft, int mostRight) {

    if (findRight < mostLeft || findLeft > mostRight)
        return identityElement;

    if (lazy[node] != defaultLazyValue) {

        updateTreeWithLazyVal(seg[node], lazy[node], mostLeft, mostRight);

        //push lazy val to its children
        if (mostLeft != mostRight) {
            updateLazyValue(lazy[leftChild(node)], lazy[node]);
            updateLazyValue(lazy[rightChild(node)], lazy[node]);
        }

        lazy[node] = defaultLazyValue;
    }


    if (findRight >= mostRight && findLeft <= mostLeft)
        return seg[node];

    return calculateParent(find(leftChild(node), findLeft, findRight, mostLeft, (mostLeft + mostRight) / 2),
        find(rightChild(node), findLeft, findRight, 1 + ((mostLeft + mostRight) / 2), mostRight));


}

type query(int findLeft, int findRight) {

    if (findLeft > findRight)
        return identityElement;

    findLeft = max(findLeft, 0);
    findRight = min(findRight, len - 1);


    return find(0, findLeft, findRight, 0, len - 1);
}



void update(int node, int left, int right, type addValue, int mostLeft, int mostRight) {
    if (right < mostLeft || left > mostRight)
        return;

    if (right >= mostRight && left <= mostLeft) {
        updateLazyValue(lazy[node], addValue);
        updateTreeWithLazyVal(seg[node], lazy[node], mostLeft, mostRight);
        
        //push lazy val to its children
        if (mostLeft != mostRight) {
            updateLazyValue(lazy[leftChild(node)], lazy[node]);
            updateLazyValue(lazy[rightChild(node)], lazy[node]);
        }
        lazy[node] = defaultLazyValue;

        return;
    }


    if (lazy[node] != defaultLazyValue) {

        updateTreeWithLazyVal(seg[node], lazy[node], mostLeft, mostRight);
        
        //push lazy val to its children
        if (mostLeft != mostRight) {
            updateLazyValue(lazy[leftChild(node)], lazy[node]);
            updateLazyValue(lazy[rightChild(node)], lazy[node]);
        }

        lazy[node] = defaultLazyValue;
    }

    update(leftChild(node), left, right, addValue, mostLeft, (mostLeft + mostRight) / 2);
    update(rightChild(node), left, right, addValue, 1 + ((mostLeft + mostRight) / 2), mostRight);

    seg[node] = calculateParent(seg[leftChild(node)], seg[rightChild(node)]);
}

/**
 * @brief 0 <= index < len
 *
 * @param index
 * @param newValue
 */
void update(int left, int right, type addValue) {

    left = max(left, 0);
    right = min(right, len - 1);

    update(0, left, right, addValue, 0, len - 1);
}


int main() {
    getInput();
    init();

    int cont = 1;

    while (cont) {

        cout << "1 for query, 2 for update, 0 to stop " << endl;
        cin >> cont;

        if (cont == 1) {
            int a, b;
            cin >> a >> b;
            cout << query(a, b) << endl;
        }
        else if (cont == 2) {
            int a, b, c;
            cin >> a >> b >> c;
            update(a, b, c);
        }



    }

    return 0;
}

