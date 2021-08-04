#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

// display array
void disp(int *arr, int n) {
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
}
// display heap
void disp_heap(int *arr, int n) {
    int ctr = 0, layer = 0;
    for (int t = n; t > 1; t >>= 1)
        ctr++;
    for (int i = 0; i < n; ++i) {
        if (i + 1 == 1 << layer) {
            printf("\n");
            for (int j = ctr - layer; j > 0; --j)
                printf("  ");
            layer++;
        }
        printf("%2d  ", arr[i]);
    }
}
/*
	冒泡排序：
	在给定范围内从左到右，每次出现相邻元素逆序就进行交换，最终最大的元素在该范围的最右边
	然后缩小范围，重复上述步骤，进行 n - 1 趟排序，排序完成
	优化后，通过判断一趟排序是否出现交换，若未出现交换，则元素已经顺序排列，退出排序
	复杂度：O(n^2)
*/
void bubble_sort(int *arr, int n) {
    int i, j, t, swapped = 1;
    // n - 1 趟排序
    for (i = n - 1; i > 0 && swapped; --i) {
        // 将是否交换的标志置为 0
        swapped = 0;
        // 从左到右依次对比相邻元素，范围为 [0, n - 1]
        for (j = 0; j < i; ++j) {
            // 若相邻元素逆序，则进行交换，同时将是否交换的标志置为 1
            if (arr[j] > arr[j + 1]) {
                t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
                swapped = 1;
            }
        }
    }
}
/*
	选择排序：
	在给定范围内，先假定 k 为最小元素下标，然后从左到右遍历，遇到更大的元素则更新下标
	若在第 i 次循环，下标在遍历后有更新，则将该下标对应元素与第 i 个元素交换
	从而找到 [i, n - 1] 中最小的元素并换到 [i] 的位置
	然后缩小范围，重复上述步骤，进行 n - 1 趟排序，排序完成
	复杂度：O(n^2)
*/
void selection_sort(int *arr, int n) {
    int i, j, k, t;
    // n - 1 趟排序
    for (i = 0; i < n - 1; ++i) {
        // 假定 k = i 为最小元素下标
        k = i;
        // 从 [i + 1] 开始，从左向右遍历，找到最小元素并更新最小元素下标
        for (j = i + 1; j < n; ++j)
            if (arr[k] > arr[j])
                k = j;
        // 若 k 发生改变，交换 [k] 与 [i]
        if (k != i) {
            t = arr[k];
            arr[k] = arr[i];
            arr[i] = t;
        }
    }
}
/*
	直接插入排序：
	在已排序好的序列中，从右往左遍历
	将所有比当前元素大的向右挪动一位，直到遇到比当前元素小的
	每次挪动都覆盖后一个的位置，因此要先保存当前最右边的元素
	在挪动完成后，令存储的最右边的元素填补到空出的位置，从而完成 [0, i] 的排序
	重复上述步骤，进行 n - 1 趟排序，排序完成
	复杂度：O(n^2)
*/
void insertion_sort(int *arr, int n) {
    int i, j, t;
    // n - 1 趟排序
    for (i = 1; i < n; ++i) {
        // 挂起 [i] 的数据
        t = arr[i];
        // 右挪所有比 [i] 大的元素
        for (j = i - 1; j >= 0 && arr[j] > t; --j)
            arr[j + 1] = arr[j];
        // 将 [i] 填入空出的位置
        arr[j + 1] = t;
    }
}
/*
	希尔排序：
	原理类似直接插入排序，但排序的元素之间的间隔为 gap，gap 每趟排序减小一倍
	在 gap 减小的排序过程中，元素将逐渐变为大致有序
	当 gap == 1 时，进行一趟直接插入排序，排序完成
	复杂度：O(n(logn)^2)
*/
void shell_sort(int *arr, int n) {
	int i, j, k, gap, t;
    // 初始化 gap = n >> 1，每趟直接插入排序后，gap >>= 1
	for (gap = n >> 1; gap > 0; gap >>= 1)
        // 由于间隔为 gap，将有多次排序的起点，分别是 [0, gap - 1]
        for (i = 0; i < gap; ++i)
            // 此处属于直接插入排序，原理类似直接插入排序
            for (j = i + gap; j < n; j += gap) {
                t = arr[j];
                for (k = j - gap; k >= i && arr[k] > t; k -= gap)
                    arr[k + gap] = arr[k];
                arr[k + gap] = t;
            }
}
/*
	堆调整函数：
	若左右子节点不为空，且发现最大的子节点的值大于当前值，与之交换，并调整该子节点
	否则，若左子节点不为空，且左子节点的值发于当前值，与之交换，并调整左子节点
	否则，不做调整
*/
void heapify(int *arr, int n, int i) {
    int t;
    // 计算左右子节点的下标
    int l = (i << 1) + 1;
    int r = l + 1;
    // 左子节点存在
    if (r < n) {
        // 左右子节点均存在，但右子节点的值更大
        if (arr[l] < arr[r]) {
            // 若该子节点的值大于当前值，进行交换，并调整该子节点
            if (arr[i] < arr[r]) {
                t = arr[i];
                arr[i] = arr[r];
                arr[r] = t;
                heapify(arr, n, r);
            }
        // 左右子节点均存在，但左子节点的值更大
        } else {
            // 若该子节点的值大于当前值，进行交换，并调整该子节点
            if (arr[i] < arr[l]) {
                t = arr[i];
                arr[i] = arr[l];
                arr[l] = t;
                heapify(arr, n, l);
            }
        }
    // 右子节点不存在，但左子节点存在
    } else if (l < n) {
        // 若该子节点的值大于当前值，进行交换，并调整该子节点
        if (arr[i] < arr[l]) {
            t = arr[i];
            arr[i] = arr[l];
            arr[l] = t;
            heapify(arr, n, l);
        }
    }
}
/*
	堆排序：
	将数组初始化成为大顶堆
	将堆顶元素与 [0, i] 范围内的最右边元素进行交换
	因堆顶元素被更改，重新调整堆顶
	重复上述步骤，进行 n - 1 趟排序，排序完成
	复杂度：O(nlogn)
*/
void heap_sort(int *arr, int n) {
    int i, t;
    // 初始化一个大顶堆
    for (i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);
   	// 将堆顶元素与 [0, i] 范围内的最右边元素进行交换，共进行 n - 1 趟
    for (i = n - 1; i >= 0; --i) {
        t = arr[i];
        arr[i] = arr[0];
        arr[0] = t;
        heapify(arr, i, 0);
    }
}
/*
	归并排序：
	若序列长度不大于 2，进行简单的判断和交换
	若序列长度大于 2，将序列二分为两个子序列进行递归的归并排序
	子序列归并完成后，开辟新空间
	将两个有序的子序列写入新的空间，数据根据两个子序列元素的大小关系进行交替写入
	交替写入完成后，将剩余未写入的元素直接补到最后
	将新空间的数据复制到原序列，排序完成
	复杂度：O(nlogn)
*/
void merge_sort(int *arr, int n) {
    int t;
    // 序列长度不大于 2
    if (n <= 2) {
        if (n == 2 && arr[0] > arr[1]) {
            t = arr[0];
            arr[0] = arr[1];
            arr[1] = t;
        }
    // 序列长度大于 2
    } else {
        int l, r, left_range, right_range, *left_ptr, *right_ptr, *tmp_arr;
        // 计算左右子序列的范围
        left_range = n >> 1;
        right_range = n - left_range;
        // 计算左右子序列的指针
        left_ptr = arr;
        right_ptr = arr + left_range;
        // 递归使用归并排序进行子序列的排序
        merge_sort(left_ptr, left_range);
        merge_sort(right_ptr, right_range);
        // 分配等大小的空间进行数据的归并
        tmp_arr = (int *)malloc(n * sizeof(int));
        // 初始化左右子序列的数据下标为 0
        t = l = r = 0;
        // 遍历子序列进行数据归并
        while (l < left_range && r < right_range) {
            while (l < left_range && left_ptr[l] <= right_ptr[r])
                tmp_arr[t++] = left_ptr[l++];
            while (r < right_range && right_ptr[r] <= left_ptr[l])
                tmp_arr[t++] = right_ptr[r++];
        }
        // 将剩余未写入的元素直接补到最后
        while (l < left_range)
            tmp_arr[t++] = left_ptr[l++];
        while (r < right_range)
            tmp_arr[t++] = right_ptr[r++];
        // 将新空间的数据复制到原序列
        for (t = 0; t < n; t++)
            arr[t] = tmp_arr[t];
        // 释放分配的空间
        free(tmp_arr);
    }
}
/*
	快速排序：
	若序列长度不大于 1，不做任何处理
	若序列长度大于 1，找到所有比 [0] 小的元素放在左边，所有比 [0] 大的元素放在右边
	然后递归对两个子序列进行快速排序
	将 [0] 挂起，同时置左下标 l 为 0，右下标 r 为 n - 1
	先让 r 向左遍历，直到遇到比 [0] 小的元素，将该元素复制到 l 所在位置，l 右移
	再让 l 向右遍历，直到遇到比 [0] 大的元素，将该元素复制到 r 所在位置，r 左移
	重复以上过程，直至 l >= r，这时候应有 l == r，将挂起的 [0] 填至 [l](即[r])
	递归对 [l](即[r]) 两边的序列进行快速排序，最终完成排序
	复杂度：O(nlogn)
*/
void quick_sort(int *arr, int n) {
    int l = 0, r = n - 1, t = *arr;
    // 序列长度不大于 1，不做任何处理
    if (n <= 1)
        return ;
   	// 序列长度大于 1，找到所有比 [0] 小的元素放在左边，所有比 [0] 大的元素放在右边
    while (l < r) {
        // 让 r 向左遍历，直到遇到比 [0] 小的元素
        while (l < r && t < arr[r])
            --r;
        // 将该元素复制到 l 所在位置，l 右移
        if (l < r)
            arr[l++] = arr[r];
        // 让 l 向右遍历，直到遇到比 [0] 大的元素
        while (l < r && arr[l] < t)
            ++l;
        // 将该元素复制到 r 所在位置，r 左移
        if (l < r)
            arr[r--] = arr[l];
    }
    // 这时 l == r，将挂起的 [0] 填至 [l](即[r])
    arr[l] = t;
    // 递归对 [l](即[r]) 两边的序列进行快速排序
    quick_sort(arr, l);
    quick_sort(arr + l + 1, n - l - 1);
}
/*
	基数排序：
	基数排序是特殊的桶排序，此处使用二进制位进行排序
	找到所有数据中最大的数据，并计算其二进制数码的最高位，从而得到分配收集的趟数
	由于有 0 和 1 两个桶，需要分配两个桶的内存 [2][n]，同时还需要元素计数器 ctr[2]
	对于每一趟，首先置 0 和 1 两个桶的计数为 ctr[0] = ctr[1] = 0
	然后计算数字某个二进制位的数码 (0/1)，并将数据防入对应的桶，同时对应的桶的元素计数 + 1
	将分配到 0 和 1 两个桶的数据重新收集到原序列
	每次分配和收集，数据都会趋于有序，同时位运算的运算速度很快，排序速度也能更高效
	多次分配与收集过后，序列达到有序，完成排序
	复杂度：O(nk)
*/
void radix_sort(int *arr, int n) {
    char mark;
    int i, j, k, max_value = *arr, *p, *r, zero_num, range;
    // 有 0 和 1 两个桶，分配两个桶的内存 [2][n]
    int **radix = (int **)malloc(2 * sizeof(int *));
    for (i = 0; i < 2; ++i)
        radix[i] = (int *)malloc(n * sizeof(int));
    // 元素计数器 ctr[2]，用于记录两个桶的元素个数
    int *ctr = (int *)malloc(2 * sizeof(int));
    // 找到所有数据的最大值
    for (i = 0; i < n; ++i)
        if (max_value < arr[i])
            max_value = arr[i];
   	// 计算数据中最大值的二进制数码的最高位，从而得到分配收集的趟数 range
    for (range = 0; max_value; ++range)
        max_value >>= 1;
    // 进行 range 次分配收集
    for (i = 0; i < range; ++i) {
        // 元素计数器清零
        ctr[0] = ctr[1] = 0;
        // 计算数字某个二进制位的数码 (0/1)，并将数据防入对应的桶，同时对应的桶的元素计数 + 1
        for (j = 0; j < n; ++j) {
            mark = (arr[j] & (1 << i)) >> i;
            radix[mark][ctr[mark]++] = arr[j];
        }
        // 将分配到 0 和 1 两个桶的数据重新收集到原序列
        for (j = 0, p = arr, r = radix[j]; j < 2; r = radix[++j])
            for (k = 0; k < ctr[j]; ++k)
                *p++ = r[k];
    }
    // 释放两个桶和计数器的空间
    for (i = 0; i < 2; i++)
        free(radix[i]);
    free(ctr);
}
/*
	计数排序：
	找到数据中的最大值与最小值，从而得到数据范围
	根据数据范围分配一段内存，将数据的值直接映射到这段内存的索引值，令索引所在的值 + 1
	从这段内存最低位开始遍历，索引所在的值代表了元素出现的次数
	按照元素顺序出现的次数，将元素直接写入原序列，完成排序
	复杂度：O(n)
*/
void counting_sort(int *arr, int n) {
    int i, j, t, range, *tmp_arr, *p = arr,  _min = *arr, _max = *arr;
    // 找到数据的最大值和最小值
    for (i = 1; i < n; ++i) {
        if (_min > arr[i])
            _min = arr[i];
        else if (_max < arr[i])
            _max = arr[i];
    }
    // 计算数据的范围，从而能够将数据的值直接映射成新内存的索引
    range = _max - _min + 1;
    // 分配新内存，同时对内存清零，其中，分配的内存大小就是数据的范围
    tmp_arr = (int *)calloc(range, sizeof(int));
    // 将数据的值直接映射成对应索引，令索引所在的值 + 1
    for (i = 0; i < n; ++i)
        ++tmp_arr[arr[i] - _min];
    // 按照元素顺序出现的次数，将元素直接写入原序列，完成排序
    for (i = 0; i < range; ++i) {
        for (j = tmp_arr[i], t = i + _min; j > 0; --j)
            *p++ = t;
    }
    // 释放分配的空间
    free(tmp_arr);
}

// functions' information
void (*func[])(int *, int) = {bubble_sort, selection_sort, insertion_sort, shell_sort, heap_sort, merge_sort, quick_sort, radix_sort, counting_sort};
char *func_name[] = {"bubble sort", "selection sort", "insertion sort", "shell sort", "heap sort", "merge sort", "quick sort", "radix sort", "counting sort"};
double func_time[9] = {0};
int func_num = 9;

// to sort an array and record its occupied time
double rec(int *arr, int n, int func_index) {
    // allocate memories to a new pointer and use memcpy to copy data to test speed
    int *test_arr = (int *)malloc(n * sizeof(int));
    // calculate occupied time
    double occupied_time;
    // initialize test array
    memcpy(test_arr, arr, n * sizeof(int));
    // record start time and finish time
    clock_t start, finish;
    // record the time for starting sorting
    start = clock();
    // invoke the sorting function
    func[func_index](test_arr, n);
    // record the time for finishing sorting
    finish = clock();
    // calculate its occupied time
    occupied_time = (double)(finish - start) / CLOCKS_PER_SEC;
    // return occupied time
    return occupied_time;
}

int main() {
    /* clear screen and set a random time seed */
    system("cls");
    srand((unsigned int)time(NULL));


    /* test if a sorting algorithm is correct */

    // int n = 20;
    // int *arr = (int *)malloc(n * sizeof(int));
    // for (int i = 0; i < n; i++)
    //     arr[i] = rand() % 100;
    // disp(arr, n);
    // counting_sort(arr, n);
    // disp(arr, n);
    // free(arr);


    /* test the speed of a sorting algorithm */

    int i, j;
    int data_num = 8000, test_times = 100;
    int *arr = (int *)malloc(data_num * sizeof(int));
    printf("Total data number: %d\n\n", data_num);
    printf("Total test times: %d\n\n", test_times);
    printf("\rCurrent progress: %5.2f%%", (double)0);
    for (i = 0; i < test_times;) {
        for (j = 0; j < data_num; ++j) arr[j] = rand() % 100;
        for (j = 0; j < func_num; ++j) func_time[j] += rec(arr, data_num, j);
        printf("\rCurrent progress: %5.2f%%", 100.0 * ++i / test_times);
    }
    for (j = 0; j < func_num; ++j) {
        // func_time[j] /= test_times;
        printf("\n%14s takes %.3f s", func_name[j], func_time[j]);
    }
    free(arr);
    getchar();
}
