include "assert.f95"

module deque_library
    implicit none
    private

    public deque, deque_init, deque_length, deque_capacity, deque_get, deque_set
    public deque_reserve, deque_resize, deque_shrink
    public deque_push_back, deque_push_front, deque_pop_back, deque_pop_front

    type :: deque
        private
        real, allocatable :: data(:)
        integer :: shift = 0, length = 0
    end type deque

contains
    function deque_init() result (dq)
        type(deque) :: dq
        allocate(dq%data(4))
    end function

    function deque_length(dq) result (length)
        type(deque), intent(in) :: dq
        integer :: length
        length = dq%length
    end function

    function deque_capacity(dq) result (capacity)
        type(deque), intent(in) :: dq
        integer :: capacity
        capacity = size(dq%data)
    end function

    function real_index(dq, i) result (idx)
        type(deque), intent(in) :: dq
        integer, intent(in) :: i
        integer :: idx

        idx = i + dq%shift
        idx = mod(idx - 1, size(dq%data)) + 1
        idx = mod(idx - 1 + size(dq%data), size(dq%data)) + 1 ! To handle negative values
    end function

    function deque_get(dq, i) result (item)
        type(deque), intent(in) :: dq
        integer, intent(in) :: i
        real :: item
        item = dq%data(real_index(dq, i))
    end function

    subroutine deque_set(dq, i, item)
        type(deque), intent(inout) :: dq
        integer, intent(in) :: i
        real, intent(in) :: item
        dq%data(real_index(dq, i)) = item
    end subroutine

    subroutine realloc(dq, new_size)
        type(deque), intent(inout) :: dq
        integer, intent(in) ::  new_size

        real, allocatable :: tmp_data(:)
        integer :: before_wrap

        call move_alloc(dq%data, tmp_data)
        allocate(dq%data(new_size))

        before_wrap = min(dq%length, size(tmp_data) - dq%shift)
        dq%data(1:before_wrap) = tmp_data(dq%shift+1:dq%shift+before_wrap)
        dq%data(before_wrap+1:dq%length) = tmp_data(1:dq%length-before_wrap)

        deallocate(tmp_data)
        dq%shift = 0
    end subroutine

    subroutine deque_reserve(dq, capacity)
        type(deque), intent(inout) :: dq
        integer, intent(in) :: capacity
        integer :: new_size

        new_size = capacity
        if (new_size > size(dq%data)) then
            new_size = max(new_size, 2 * size(dq%data))
            call realloc(dq, new_size)
        end if
    end subroutine

    subroutine deque_resize(dq, length)
        type(deque), intent(inout) :: dq
        integer, intent(in) :: length

        if (length > size(dq%data)) then
            call deque_reserve(dq, length)
        end if

        dq%length = length
    end subroutine

    subroutine deque_shrink(dq)
        type(deque), intent(inout) :: dq

        if (dq%length /= size(dq%data) .and. size(dq%data) > 4) then
            call realloc(dq, max(dq%length, 4))
        end if
    end subroutine

    subroutine deque_push_back(dq, value)
        type(deque), intent(inout) :: dq
        real, intent(in) :: value

        call deque_resize(dq, dq%length + 1)
        call deque_set(dq, dq%length, value)
    end subroutine

    subroutine deque_push_front(dq, value)
        type(deque), intent(inout) :: dq
        real, intent(in) :: value

        call deque_resize(dq, dq%length + 1)
        call deque_set(dq, size(dq%data), value)
        dq%shift = mod(dq%shift - 1 + size(dq%data), size(dq%data))
    end subroutine

    function deque_pop_back(dq) result (val)
        type(deque), intent(inout) :: dq
        real :: val

        val = deque_get(dq, dq%length)
        dq%length = dq%length - 1
    end function

    function deque_pop_front(dq) result (val)
        type(deque), intent(inout) :: dq
        real :: val

        val = deque_get(dq, 1)
        dq%shift = mod(dq%shift + 1, size(dq%data))
        dq%length = dq%length - 1
    end function
end module

program deque_test
    use deque_library
    implicit none

    type(deque) :: dq
    dq = deque_init()

    call assert(deque_length(dq) == 0 .and. deque_capacity(dq) == 4)
    call deque_push_back(dq, 1.0)
    call assert(deque_length(dq) == 1 .and. deque_capacity(dq) == 4)
    call deque_push_back(dq, 2.0)
    call assert(deque_length(dq) == 2 .and. deque_capacity(dq) == 4)
    call deque_push_front(dq, 0.5)
    call assert(deque_length(dq) == 3 .and. deque_capacity(dq) == 4)
    call deque_push_back(dq, 3.0)
    call assert(deque_length(dq) == 4 .and. deque_capacity(dq) == 4)
    call deque_push_front(dq, 0.25)
    call assert(deque_length(dq) == 5 .and. deque_capacity(dq) == 8)
    call deque_reserve(dq, 10)
    call assert(deque_length(dq) == 5 .and. deque_capacity(dq) == 16)
    call deque_push_back(dq, 4.0)
    call assert(deque_length(dq) == 6 .and. deque_capacity(dq) == 16)
    call deque_reserve(dq, 33)
    call assert(deque_length(dq) == 6 .and. deque_capacity(dq) == 33)
    call deque_resize(dq, 7)
    call assert(deque_length(dq) == 7 .and. deque_capacity(dq) == 33)
    call deque_shrink(dq)
    call assert(deque_length(dq) == 7 .and. deque_capacity(dq) == 7)
    call deque_set(dq, 7, 5.25)

    call assert(deque_get(dq, 1) == 0.25)
    call assert(deque_get(dq, 2) == 0.5)
    call assert(deque_get(dq, 3) == 1.0)
    call assert(deque_get(dq, 4) == 2.0)
    call assert(deque_get(dq, 5) == 3.0)
    call assert(deque_get(dq, 6) == 4.0)
    call assert(deque_get(dq, 7) == 5.25)

    call assert(deque_pop_front(dq) == 0.25)
    call assert(deque_pop_front(dq) == 0.5)
    call assert(deque_pop_front(dq) == 1.0)
    call assert(deque_pop_back(dq) == 5.25)
    call assert(deque_pop_back(dq) == 4.0)
    call assert(deque_pop_back(dq) == 3.0)
    call assert(deque_length(dq) == 1 .and. deque_capacity(dq) == 7 .and. deque_get(dq, 1) == 2.0)

    print *, 'Tests passed!'
end program
