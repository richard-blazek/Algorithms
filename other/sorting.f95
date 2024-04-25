include "assert.f95"

module sorting_library
    implicit none
    private
    public bubble_sort, selection_sort, insertion_sort ! Stupid
    public quick_sort, merge_sort, heap_sort           ! Clever
    public lsd_radix_sort, msd_radix_sort, three_way_quick_sort ! Used for strings

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
        real :: tmp

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
        real, intent(inout) :: dst(:)
        real, intent(in) :: src1(:), src2(:)
        integer :: i, j, k

        i = 1
        j = 1
        do k = 1, size(dst)
            if (i > size(src1) .or. (j <= size(src2) .and. src2(j) < src1(i))) then
                dst(k) = src2(j)
                j = j + 1
            else
                dst(k) = src1(i)
                i = i + 1
            end if
        end do
    end subroutine

    subroutine merge_sort(array)
        real, intent(inout) :: array(:)
        real :: tmp(size(array))
        integer :: step, i, end1, end2

        step = 1
        do while (step < size(array))
            do i = 1, size(array), 2 * step
                end1 = min(i + step - 1, size(array))
                end2 = min(i + step * 2 - 1, size(array))
                call merge(tmp(i : end2), array(i : end1), array(end1 + 1 : end2))
            end do
            array = tmp
            step = step * 2
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

    function heap_remove(heap) result (val)
        real, intent(inout) :: heap(:)
        real :: val
        integer :: i

        val = heap(1)
        heap(1) = heap(size(heap))

        i = 1
        do while (.true.)
            if (i*2+1 <= size(heap)-1 .and. heap(i*2+1) > heap(i*2) .and. heap(i*2+1) > heap(i)) then
                call swap(heap(i*2+1), heap(i))
                i = i*2+1
            else if (i*2 <= size(heap)-1 .and. heap(i*2) > heap(i)) then
                call swap(heap(i*2), heap(i))
                i = i*2
            else
                exit
            end if
        end do
    end function

    subroutine heap_sort(array)
        real, intent(inout) :: array(:)
        integer :: i

        do i = 2, size(array)
            call heap_insert(array(1:i), array(i))
        end do
        do i = size(array), 1, -1
            array(i) = heap_remove(array(1:i))
        end do
    end subroutine

    subroutine radix_sort_pass(array, letter_index, starts)
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
            call radix_sort_pass(array, letter_index, starts)
        end do
    end subroutine

    recursive subroutine msd_radix_sort_nth(array, letter_index)
        character(32), intent(inout) :: array(:)
        integer, intent(in) :: letter_index
        integer :: starts(0:256), i

        call radix_sort_pass(array, letter_index, starts)

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

    subroutine three_way_partition(array, letter_index, begin, end)
        character(32), intent(inout) :: array(:)
        integer, intent(in) :: letter_index
        integer, intent(out) :: begin, end
        character(32) :: tmp
        integer :: i

        begin = 1
        end = 1
        do i = 2, size(array)
            if (array(i)(letter_index:letter_index) == array(begin)(letter_index:letter_index)) then
                tmp = array(end + 1)
                array(end + 1) = array(i)
                array(i) = tmp
                end = end + 1
            else if (array(i)(letter_index:letter_index) < array(begin)(letter_index:letter_index)) then
                tmp = array(begin)
                array(begin) = array(i)
                array(i) = array(end + 1)
                array(end + 1) = tmp
                begin = begin + 1
                end = end + 1
            end if
        end do
    end subroutine

    recursive subroutine three_way_pass(array, letter_index)
        character(32), intent(inout) :: array(:)
        integer, intent(in) :: letter_index
        integer :: begin, end

        call three_way_partition(array, letter_index, begin, end)
        if (begin > 2) then
            call three_way_pass(array(:begin-1), letter_index)
        end if
        if (end - begin > 0 .and. letter_index < 32) then
            call three_way_pass(array(begin:end), letter_index + 1)
        end if
        if (end < size(array) - 1) then
            call three_way_pass(array(end+1:), letter_index)
        end if
    end subroutine

    subroutine three_way_quick_sort(array)
        character(32), intent(inout) :: array(:)
        call three_way_pass(array, 1)
    end subroutine
end module

module testing_tools
    implicit none
    private
    public test_sort_real, test_sort_str

    abstract interface
        subroutine sort_real(array)
            real, intent(inout) :: array(:)
        end subroutine
    end interface

    abstract interface
        subroutine sort_str(array)
            character(32), intent(inout) :: array(:)
        end subroutine
    end interface

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

    subroutine test_sort_real(sort)
        procedure(sort_real) :: sort
        real :: arr (137)

        call random_number(arr)
        call sort(arr)
        call assert(is_sorted_real(arr))
        call sort(arr)
        call assert(is_sorted_real(arr))
    end subroutine

    subroutine test_sort_str(sort)
        procedure(sort_str) :: sort
        character(32) :: arr (137)

        call random_str(arr)
        call sort(arr)
        call assert(is_sorted_str(arr))
        call sort(arr)
        call assert(is_sorted_str(arr))
    end subroutine
end module

program sorting_test
    use sorting_library
    use testing_tools
    implicit none

    integer :: i

    call random_seed()

    do i = 1, 400
        call test_sort_real(bubble_sort)
        call test_sort_real(selection_sort)
        call test_sort_real(insertion_sort)
        call test_sort_real(quick_sort)
        call test_sort_real(merge_sort)
        call test_sort_real(heap_sort)

        call test_sort_str(lsd_radix_sort)
        call test_sort_str(msd_radix_sort)
        call test_sort_str(three_way_quick_sort)
    end do

    print *, 'Tests passed!'
end program
