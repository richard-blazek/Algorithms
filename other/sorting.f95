include "assert.f95"

module sorting_library
    implicit none
    private
    public bubble_sort, selection_sort, insertion_sort
    public quick_sort, merge_sort, heap_sort
    public lsd_radix_sort, msd_radix_sort

contains
    subroutine swap(x, y)
        real, intent(inout) :: x, y
        real :: tmp
        tmp = x
        x = y
        y = tmp
    end subroutine

    subroutine bubble_sort(array)
        real, intent(inout) :: array(:)
        logical :: swapped
        integer :: i, length

        swapped = .true.
        length = size(array)

        do while (swapped)
            swapped = .false.
            do i = 1, length - 1
                if (array(i) > array(i + 1)) then
                    call swap(array(i), array(i + 1))
                    swapped = .true.
                end if
            end do
            length = length - 1
        end do
    end subroutine

    subroutine selection_sort(array)
        real, intent(inout) :: array(:)
        integer :: i, j, min

        do i = 1, size(array)
            min = i
            do j = i + 1, size(array)
                if (array(j) < array(min)) then
                    min = j
                end if
            end do

            call swap(array(i), array(min))
        end do
    end subroutine

    subroutine insertion_sort(array)
        real, intent(inout) :: array(:)
        integer :: i, dst

        do i = 1, size(array)
            dst = i
            do while (dst > 0 .and. array(dst) < array(dst - 1))
                call swap(array(dst), array(dst - 1))
                dst = dst - 1
            end do
        end do
    end subroutine

    subroutine partition(array, pivot)
        real, intent(inout) :: array(:)
        integer, intent(out) :: pivot
        integer :: i
        real ::tmp

        pivot = 1
        do i = 1, size(array)
            if (array(i) < array(pivot)) then
                tmp = array(pivot)
                array(pivot) = array(i)
                array(i) = array(pivot + 1)
                array(pivot + 1) = tmp

                pivot = pivot + 1
            end if
        end do
    end subroutine

    recursive subroutine quick_sort(array)
        real, intent(inout) :: array(:)
        integer :: pivot

        call partition(array, pivot)

        if (pivot - 1 > 1) then
            call quick_sort(array(1:pivot-1))
        end if
        if (pivot + 1 < size(array)) then
            call quick_sort(array(pivot + 1:size(array)))
        end if        
    end subroutine

    subroutine merge(dst, src1, src2)
        real, intent(out) :: dst(:)
        real, intent(in) :: src1(:), src2(:)
        integer :: i = 1, j = 1, k

        do k = 1, size(src1) + size(src2)
            if (i > size(src1) .or. src2(j) < src1(i)) then
                dst(k) = src2(j)
                j = j + 1
            else
                dst(k) = src1(i)
                i = i + 1
            end if
        end do
    end subroutine

    subroutine merge_all(src, dst, d)
        real, intent(in) :: src(:)
        real, intent(out) :: dst(:)
        integer, intent(in) :: d
        integer :: i

        do i = 1, size(src), 2 * d
            call merge(dst(i:), src(i : min(i + d - 1, size(src))), src(i + d : min(i + 2 * d - 1, size(src))))
        end do
    end subroutine

    subroutine merge_sort(array)
        real, intent(inout) :: array(:)
        real, allocatable :: tmp(:)
        integer :: step = 1

        allocate(tmp(size(array)))

        do while (step < size(array))
            call merge_all(array, tmp, step)
            call merge_all(tmp, array, step * 2)
            step = step * 4
        end do
    end subroutine

    subroutine heap_insert(heap, val)
        real, intent(inout) :: heap(:)
        real, intent(in) :: val
        integer :: i

        i = size(heap)
        heap(i) = val
        do while (i > 1 .and. heap(i) > heap(i / 2))
            call swap(heap(i), heap(i / 2))
            i = i / 2
        end do
    end subroutine

    subroutine heap_remove(heap, val)
        real, intent(inout) :: heap(:)
        real, intent(out) :: val
        integer :: i = 1, left, right, next

        val = heap(1)
        heap(1) = heap(size(heap))

        do while (i * 2 <= size(heap) - 1)
            left = i * 2
            right = i * 2 + 1
            if (right <= size(heap) - 1 .and. heap(right) < heap(left)) then
                next = right
            else
                next = left
            end if
            if (heap(next) >= heap(i)) then
                exit
            end if

            call swap(heap(next), heap(i))
            i = next
        end do
    end subroutine

    subroutine heap_sort(array)
        real, intent(inout) :: array(:)
        integer :: i

        do i = 1, size(array)
            call heap_insert(array(1:i), array(i))
        end do
        do i = size(array), 1, -1
            call heap_remove(array(1:i), array(i))
        end do
    end subroutine

    subroutine radix_sort_one_pass(array, letter_index, starts)
        character(32), intent(inout) :: array(:)
        integer, intent(in) :: letter_index
        integer, intent(out) :: starts(0:256)

        character(32) :: tmp(size(array))
        integer :: counts(0:255), i, letter

        counts = 0
        do i = 1, size(array)
            letter = ichar(array(i)(letter_index:letter_index))
            counts(letter) = counts(letter) + 1
        end do

        starts(0) = 1
        do i = 1, 256
            starts(i) = starts(i - 1) + counts(i - 1)
        end do

        do i = 1, size(array)
            letter = ichar(array(i)(letter_index:letter_index))
            tmp(starts(letter)) = array(i)
            starts(letter) = starts(letter) + 1
        end do

        array = tmp
    end subroutine

    subroutine lsd_radix_sort(array)
        character(32), intent(inout) :: array(:)
        integer :: starts(0:256), letter_index

        do letter_index = 32, 1, -1
            call radix_sort_one_pass(array, letter_index, starts)
        end do
    end subroutine

    recursive subroutine msd_radix_sort_nth(array, letter_index)
        character(32), intent(inout) :: array(:)
        integer, intent(in) :: letter_index
        integer :: starts(0:256), i

        call radix_sort_one_pass(array, letter_index, starts)

        if (letter_index < 32) then
            do i = 0, 255
                if (starts(i + 1) - starts(i) >= 2) then
                    call msd_radix_sort_nth(array(starts(i):starts(i + 1) - 1), letter_index + 1)
                end if
            end do
        end if
    end subroutine

    subroutine msd_radix_sort(array)
        character(32), intent(inout) :: array(:)
        call msd_radix_sort_nth(array, 1)
    end subroutine
end module

module testing_tools
    implicit none
    private
    public is_sorted_real, is_sorted_str
    public random_str

contains
    function is_sorted_real(array) result(sorted)
        real, intent(in) :: array(:)
        logical :: sorted
        integer :: i

        sorted = .true.
        do i = 1, size(array) - 1
            sorted = sorted .and. (array(i) <= array(i + 1))
        end do
    end function
    function is_sorted_str(array) result(sorted)
        character(32), intent(in) :: array(:)
        logical :: sorted
        integer :: i

        sorted = .true.
        do i = 1, size(array) - 1
            sorted = sorted .and. (array(i) <= array(i + 1))
        end do
    end function

    function rand_int(range) result (val)
        integer, intent(in) :: range
        integer :: val
        real :: random

        call random_number(random)
        val = floor(random * range) + 1
    end function

    subroutine random_str(array)
        character(32), intent(out) :: array(:)
        character(40) :: alphabet = "0123456789QWERTYUIOPASDFGHJKLZXCVBNM"
        integer :: i, j, letter

        do i = 1, size(array)
            do j = 1, 32
                letter = rand_int(36)
                array(i)(j:j) = alphabet(letter:letter)
            end do
        end do
    end subroutine
end module

program sorting_test
    use sorting_library
    use testing_tools
    implicit none

    real :: real_array (55)
    character(32) :: str_array (55)

    call random_seed()

    call random_number(real_array)
    call bubble_sort(real_array)
    call assert(is_sorted_real(real_array))
    call bubble_sort(real_array)
    call assert(is_sorted_real(real_array))

    call random_number(real_array)
    call selection_sort(real_array)
    call assert(is_sorted_real(real_array))
    call selection_sort(real_array)
    call assert(is_sorted_real(real_array))

    call random_number(real_array)
    call insertion_sort(real_array)
    call assert(is_sorted_real(real_array))
    call insertion_sort(real_array)
    call assert(is_sorted_real(real_array))

    call random_number(real_array)
    call quick_sort(real_array)
    call assert(is_sorted_real(real_array))
    call quick_sort(real_array)
    call assert(is_sorted_real(real_array))

    call random_number(real_array)
    call merge_sort(real_array)
    call assert(is_sorted_real(real_array))
    call merge_sort(real_array)
    call assert(is_sorted_real(real_array))

    call random_number(real_array)
    call heap_sort(real_array)
    call assert(is_sorted_real(real_array))
    call heap_sort(real_array)
    call assert(is_sorted_real(real_array))

    call random_str(str_array)
    call lsd_radix_sort(str_array)
    call assert(is_sorted_str(str_array))
    call lsd_radix_sort(str_array)
    call assert(is_sorted_str(str_array))

    call random_str(str_array)
    call msd_radix_sort(str_array)
    call assert(is_sorted_str(str_array))
    call msd_radix_sort(str_array)
    call assert(is_sorted_str(str_array))

    print *, 'Tests passed!'
end program
