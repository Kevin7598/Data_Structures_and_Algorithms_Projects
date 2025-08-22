#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP

#include <vector>
#include <stdlib.h>
#include <functional>

template<typename T>
void swap(std::vector<T> &vector, int a, int b) {
  T swap = vector[a];
  vector[a] = vector[b];
  vector[b] = swap;
}

template<typename T, typename Compare>
void bubble_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
  for (int i = static_cast<int>(vector.size()); i >= 1; i--) for (int k = 1; k < i; k++) if (comp(vector[k], vector[k - 1])) swap(vector, k, k - 1);
}

template<typename T, typename Compare>
void insertion_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
  for (int i = 1; i < static_cast<int>(vector.size()); i++) {
    T key = vector[i];
    int j = i - 1;
    while ((j >= 0) && (comp(key, vector[j]))) {
      vector[j + 1] = vector[j];
      j--;
    }
    vector[j + 1] = key;
  }
}

template<typename T, typename Compare>
void selection_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    int n = static_cast<int>(vector.size());
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (comp(vector[j], vector[minIndex])) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(vector,i, minIndex);
        }
    }
}

template<typename T, typename Compare>
void merge(std::vector<T> &vector, Compare comp = std::less<T>(), int left = 0, int mid = 0, int right = 0) {
  int length1 = mid - left + 1, length2 = right - mid;
  std::vector<T> vector1(length1);
  std::vector<T> vector2(length2);
  for (int i = 0; i < length1; i++) vector1[i] = vector[left + i];
  for (int i = 0; i < length2; i++) vector2[i] = vector[mid + 1 + i];
  int pos1 = 0, pos2 = 0, vector_pos = left;
  while ((pos1 < length1) && (pos2 < length2)) {
    if (!comp(vector2[pos2], vector1[pos1])) {
      vector[vector_pos] = vector1[pos1];
      pos1++;
    }
    else {
      vector[vector_pos] = vector2[pos2];
      pos2++;
    }
    vector_pos++;
  }
  while (pos1 < length1) {
    vector[vector_pos] = vector1[pos1];
    pos1++;
    vector_pos++;
  }
  while (pos2 < length2) {
    vector[vector_pos] = vector2[pos2];
    pos2++;
    vector_pos++;
  }
}

template<typename T, typename Compare>
void merge_sort_helper(std::vector<T> &vector, Compare comp = std::less<T>(), int left = 0, int right = 0){
  if (left < right) {
    int mid = left + (right - left) / 2;
    merge_sort_helper(vector, comp, left, mid);
    merge_sort_helper(vector, comp, mid + 1, right);
    merge(vector, comp, left, mid, right);
  }
}

template<typename T, typename Compare>
void merge_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
  merge_sort_helper(vector, comp, 0, static_cast<int>(vector.size() - 1));
}

template<typename T, typename Compare>
int partition1(std::vector<T> &vector, Compare comp = std::less<T>(), int left = 0, int right = 0) {
  T pivot = vector[left];
  std::vector<T> new_vector(right - left + 1);
  int left_pos = 0, right_pos = static_cast<int>(new_vector.size() - 1);
  for (int i = left + 1; i <= right; i++) {
    if (comp(vector[i], pivot)) new_vector[left_pos++] = vector[i];
    else new_vector[right_pos--] = vector[i];
  }
  new_vector[left_pos] = pivot;
  for (int i = 0; i < static_cast<int>(new_vector.size()); i++) vector[left + i] = new_vector[i];
  return (left + left_pos);
}

template<typename T, typename Compare>
void quick_sort_helper1(std::vector<T> &vector, Compare comp = std::less<T>(), int left = 0, int right = 0) {
  if (left < right) {
    int p = partition1(vector, comp, left, right);
    quick_sort_helper1(vector, comp, left, p - 1);
    quick_sort_helper1(vector, comp, p + 1, right);
  }
}

template<typename T, typename Compare>
void quick_sort_extra(std::vector<T> &vector, Compare comp = std::less<T>()) {
  quick_sort_helper1(vector, comp, 0, static_cast<int>(vector.size() - 1));
}

template<typename T, typename Compare>
int partition2(std::vector<T> &vector, Compare comp = std::less<T>(), int left = 0, int right = 0) {
  T pivot = vector[left];
  int first = left, last = right;
  while (first < last) {
    while ((first < last) && (!comp(vector[last], pivot))) last--;
    if (first < last) swap(vector, first, last);
    while ((first < last) && (comp(vector[first], pivot))) first++;
    if (first < last) swap(vector, first, last);
  }
  return first;
}

template<typename T, typename Compare>
void quick_sort_helper2(std::vector<T> &vector, Compare comp = std::less<T>(), int left = 0, int right = 0) {
  if (left < right) {
    int p = partition2(vector, comp, left, right);
    quick_sort_helper2(vector, comp, left, p - 1);
    quick_sort_helper2(vector, comp, p + 1, right);
  }
}

template<typename T, typename Compare>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = std::less<T>()) {
  quick_sort_helper2(vector, comp, 0, static_cast<int>(vector.size() - 1));
}

#endif //VE281P1_SORT_HPP