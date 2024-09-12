#include <iostream>
#include <vector>

using namespace std;

// As the values can be very large, a class with unsigned long long storing the magnitude and bool storing the sign is needed.
class BigInt {
private:
    unsigned long long magnitude;
    bool sign;

public:
    explicit BigInt(unsigned long long mag = 0, bool s = false) : magnitude(mag), sign(s) {}

    // Convert the long long type into BigInt.
    explicit BigInt(long long value) {
        if (value < 0) {
            sign = true;
            magnitude = static_cast<unsigned long long>(-value);
        } else {
            sign = false;
            magnitude = static_cast<unsigned long long>(value);
        }
    }

    // Convert the int type into BigInt.
    explicit BigInt(int value) {
        if (value < 0) {
            sign = true;
            magnitude = static_cast<unsigned long long>(-value);
        } else {
            sign = false;
            magnitude = static_cast<unsigned long long>(value);
        }
    }

    // Perform addition operation in BigInt.
    BigInt operator+(const BigInt& other) const {
        if (sign == other.sign) return BigInt(magnitude + other.magnitude, sign);
        else {
            if (magnitude >= other.magnitude) return BigInt(magnitude - other.magnitude, sign);
            else return BigInt(other.magnitude - magnitude, other.sign);
        }
    }

    // Perform subtraction operation in BigInt.
    BigInt operator-(const BigInt &other) const {
        if (sign != other.sign) return BigInt(magnitude + other.magnitude, sign);
        else if (magnitude >= other.magnitude) return BigInt(magnitude - other.magnitude, sign);
        else return BigInt(other.magnitude - magnitude, !sign);
    }

    // Perform multiplication operation in BigInt.
    BigInt operator*(const BigInt &other) const {
        unsigned long long result_magnitude = magnitude * other.magnitude;
        bool result_sign = sign != other.sign;
        return BigInt(result_magnitude, result_sign);
    }

    // Perform comparison operation in BigInt.
    bool operator==(const BigInt& other) const { return ((magnitude == other.magnitude) && (sign == other.sign)); }

    bool operator<(const BigInt& other) const {
        if (sign != other.sign) return sign;
        else if (sign) return (magnitude > other.magnitude);
        else return (magnitude < other.magnitude);
    }

    bool operator>(const BigInt& other) const { return (other < *this); }

    // Convert BigInt into long long for output use.
    long long toLongLong() const { return sign ? static_cast<long long>(magnitude) * -1 : static_cast<long long>(magnitude); }

    // Calculate the square of a BigInt.
    BigInt square() const { return (*this) * (*this); }
};

// A point needs to include x and y in BigInt in the coordinate.
struct coordinate{
    BigInt x;
    BigInt y;
};

// Calculate the ccw of two vectors, with negative values represented in -1, 0 represented in 0, and positive values represented in 1.
int ccw(coordinate a, coordinate b, coordinate c) {
    BigInt x1 = b.x - a.x;
    BigInt y1 = c.y - a.y;
    BigInt y2 = b.y - a.y;
    BigInt x2 = c.x - a.x;
    BigInt left = x1 * y1;
    BigInt right = y2 * x2;
    if (left < right) return -1;
    else if (left == right) return 0;
    else return 1;
}

// Find the point with the lowest y-component so that it can be put to the first position of the output vector.
coordinate find_lowest(const vector<coordinate>& point) {
    coordinate lowest = point[0];
    for (const auto & i : point) {
        if (i.y < lowest.y) lowest = i;
        if (i.y == lowest.y) if (i.x < lowest.x) lowest = i;
    }
    return lowest;
}

// Find the position of the point with the lowest y-component so that it can be put to the first position of the output vector.
int find_lowest_pos(const vector<coordinate>& point) {
    int lowest =0;
    for (int i = 0; i < static_cast<int>(point.size()); i++) {
        if (point[i].y < point[lowest].y) lowest = i;
        if (point[i].y == point[lowest].y) if (point[i].x < point[lowest].x) lowest = i;
    }
    return lowest;
}

// Quick sort algorithm
int partition(vector<coordinate> &vector, int left = 0, int right = 0, coordinate lowest = {BigInt(0), BigInt(0)}) {
    coordinate pivot = vector[left];
    int first = left, last = right;
    while (first < last) {
        while ((first < last) && (ccw(lowest, vector[last], pivot) <= 0)) last--;
        if (first < last) swap(vector[first], vector[last]);
        while ((first < last) && (ccw(lowest, pivot, vector[first]) <= 0)) first++;
        if (first < last) swap(vector[first], vector[last]);
    }
    return first;
}

// Quick sort algorithm
void quick_sort(std::vector<coordinate> &vector, int left = 0, int right = 0, coordinate lowest = {BigInt(0), BigInt(0)}) {
    if (left < right) {
        int p = partition(vector, left, right, lowest);
        quick_sort(vector, left, p - 1, lowest);
        quick_sort(vector, p + 1, right, lowest);
    }
}

// Move the lowest point to the front of the vector for the convenience of quick sort and output.
void move_to_front(vector<coordinate> &vector, coordinate element) {
    for (int i = 0; i < static_cast<int>(vector.size()); i++) if ((vector[i].x == element.x) && (vector[i].y == element.y)) {
            auto iter = vector.begin();
            for (int j = 0; j < i; j++) iter++;
            vector.erase(iter);
            vector.insert(vector.begin(), element);
            break;
        }
}

// Calculate the distance between the lowest point to the needed point so that points with same polar angles and with larger distance can be remained.
BigInt distance(coordinate a, coordinate b) { return ((a.x - b.x).square() + (a.y - b.y).square()); }

// Erase the same points and points with same polar angles.
void erase(vector<coordinate> &vector, coordinate lowest, int lowest_pos) {
    int pos = 0;
    while (pos < static_cast<int>(vector.size()) - 1) {
        if (ccw(lowest, vector[pos], vector[pos + 1]) == 0) {
            if (distance(lowest, vector[pos]) < distance(lowest, vector[pos + 1])) {
                if (pos != lowest_pos) {
                    auto iter = vector.begin();
                    for (int j = 0; j < pos; j++) iter++;
                    vector.erase(iter);
                }
                else pos++;
            }
            else if ((vector[pos].x == vector[pos + 1].x) &&  (vector[pos].y == vector[pos + 1].y)) {
                if (pos != lowest_pos) {
                    auto iter = vector.begin();
                    for (int j = 0; j < pos; j++) iter++;
                    vector.erase(iter);
                }
                else {
                    auto iter = vector.begin();
                    for (int j = 0; j < pos + 1; j++) iter++;
                    vector.erase(iter);
                }
            }
            else {
                if (pos + 1 != lowest_pos) {
                    auto iter = vector.begin();
                    for (int j = 0; j <= pos; j++) iter++;
                    vector.erase(iter);
                }
                else pos++;
            }
        }
        else pos++;
    }
}

int main(){
    // Part 1: Input the values so that the points can be stored in BigInt.
    int point_num;
    cin >> point_num;
    if (!point_num) return 0;
    vector<coordinate> point;
    for (int i = 0; i < point_num; i++) {
        long long x, y;
        cin >> x >> y;
        BigInt int1(x), int2(y);
        point.push_back({int1, int2});
    }
    // Part 2: Sort the points so that the points are in counterclockwise order and the same ones and those with same polar angles can be erased.
    coordinate lowest = find_lowest(point);
    move_to_front(point, lowest);
    quick_sort(point, 1, static_cast<int>(point.size()) - 1, lowest);
    int lowest_pos = find_lowest_pos(point);
    erase(point, lowest, lowest_pos);
    // Part 3: Select the needed points through the Graham's scan and output the results.
    vector<coordinate> selected_point;
    for (auto i : point) {
        while (selected_point.size() > 1)
            if (ccw(selected_point[selected_point.size() - 2], selected_point[selected_point.size() - 1], i) <= 0) selected_point.pop_back();
            else break;
        selected_point.push_back(i);
    }
    for (auto & i : selected_point) cout << i.x.toLongLong() << " " << i.y.toLongLong() << endl;
}