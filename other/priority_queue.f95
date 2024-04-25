include "assert.f95"

module priority_queue_library
    implicit none
    private

    public priority_queue
    public priority_queue_init, priority_queue_insert, priority_queue_delete_top
    public priority_queue_is_empty, priority_queue_top_identity, priority_queue_top_priority

    type :: priority_queue
        private
        integer, allocatable :: identity(:)
        real, allocatable :: priority(:)
        integer :: count
    end type priority_queue

contains
    function priority_queue_init() result(pq)
        type(priority_queue) :: pq

        allocate(pq%identity(8))
        allocate(pq%priority(8))
        pq%count = 0
    end function

    subroutine swap(pq, i, j)
        type(priority_queue), intent(inout) :: pq
        integer, intent(in) :: i, j
        integer :: tmp_i
        real :: tmp_r

        tmp_i = pq%identity(i)
        pq%identity(i) = pq%identity(j)
        pq%identity(j) = tmp_i

        tmp_r = pq%priority(i)
        pq%priority(i) = pq%priority(j)
        pq%priority(j) = tmp_r
    end subroutine

    subroutine bubble_up(pq, index)
        type(priority_queue), intent(inout) :: pq
        integer, intent(in) :: index
        integer :: i

        i = index
        do while (i / 2 > 0 .and. pq%priority(i) <= pq%priority(i / 2))
            call swap(pq, i, i / 2)
            i = i / 2
        end do
    end subroutine

    subroutine bubble_down(pq, index)
        type(priority_queue), intent(inout) :: pq
        integer, intent(in) :: index
        integer :: i, left, right, next

        i = index
        do while (i * 2 <= pq%count)
            left = i * 2
            right = i * 2 + 1
            if (right <= pq%count .and. pq%priority(right) < pq%priority(left)) then
                next = right
            else
                next = left
            end if
            if (pq%priority(next) >= pq%priority(i)) then
                exit
            end if

            call swap(pq, i, next)
            i = next
        end do
    end subroutine

    subroutine resize(pq, new_size)
        type(priority_queue), intent(inout) :: pq
        integer, intent(in) :: new_size
        integer, allocatable :: tmp_ident(:)
        real, allocatable :: tmp_prior(:)
        
        integer :: copied
        copied = min(size(pq%identity), new_size)

        allocate(tmp_ident(new_size))
        tmp_ident(1:copied) = pq%identity
        deallocate(pq%identity)
        call move_alloc(tmp_ident, pq%identity)

        allocate(tmp_prior(new_size))
        tmp_prior(1:copied) = pq%priority
        deallocate(pq%priority)
        call move_alloc(tmp_prior, pq%priority)
    end subroutine

    subroutine priority_queue_insert(pq, identity, priority)
        type(priority_queue), intent(inout) :: pq
        integer, intent(in) :: identity
        real, intent(in) :: priority

        pq%count = pq%count + 1
        if (pq%count > size(pq%identity)) then
            call resize(pq, size(pq%identity) * 2)
        end if

        pq%identity(pq%count) = identity
        pq%priority(pq%count) = priority
        call bubble_up(pq, pq%count)
    end subroutine

    function priority_queue_is_empty(pq) result(empty)
        type(priority_queue), intent(inout) :: pq
        logical :: empty
        empty = (pq%count == 0)
    end function

    subroutine priority_queue_delete_top(pq)
        type(priority_queue), intent(inout) :: pq

        pq%identity(1) = pq%identity(pq%count)
        pq%priority(1) = pq%priority(pq%count)
        pq%count = pq%count - 1
        call bubble_down(pq, 1)

        if (pq%count < size(pq%identity) / 4 .and. size(pq%identity) > 8) then
            call resize(pq, size(pq%identity) / 2)
        end if
    end subroutine

    function priority_queue_top_identity(pq) result(identity)
        type(priority_queue), intent(inout) :: pq
        integer :: identity
        identity = pq%identity(1)
    end function

    function priority_queue_top_priority(pq) result(priority)
        type(priority_queue), intent(inout) :: pq
        real :: priority
        priority = pq%priority(1)
    end function
end module

program priority_queue_test
    use priority_queue_library
    implicit none

    type(priority_queue) :: priorityq
    priorityq = priority_queue_init()
    call assert(priority_queue_is_empty(priorityq))

    call priority_queue_insert(priorityq,  4,  4 / 8.0)
    call priority_queue_insert(priorityq,  8,  8 / 8.0)
    call priority_queue_insert(priorityq,  1,  1 / 8.0)
    call priority_queue_insert(priorityq,  5,  5 / 8.0)
    call priority_queue_insert(priorityq,  2,  2 / 8.0)
    call priority_queue_insert(priorityq,  4,  4 / 8.0)
    call priority_queue_insert(priorityq, 11, 11 / 8.0)
    call priority_queue_insert(priorityq,  6,  6 / 8.0)
    call priority_queue_insert(priorityq,  3,  3 / 8.0)
    call assert(.not. priority_queue_is_empty(priorityq))

    call assert(priority_queue_top_identity(priorityq) == 1)
    call assert(priority_queue_top_priority(priorityq) == 1 / 8.0)
    call priority_queue_delete_top(priorityq)

    call assert(priority_queue_top_identity(priorityq) == 2)
    call assert(priority_queue_top_priority(priorityq) == 2 / 8.0)
    call priority_queue_delete_top(priorityq)

    call assert(priority_queue_top_identity(priorityq) == 3)
    call assert(priority_queue_top_priority(priorityq) == 3 / 8.0)
    call priority_queue_delete_top(priorityq)

    call assert(priority_queue_top_identity(priorityq) == 4)
    call assert(priority_queue_top_priority(priorityq) == 4 / 8.0)
    call priority_queue_delete_top(priorityq)

    call assert(priority_queue_top_identity(priorityq) == 4)
    call assert(priority_queue_top_priority(priorityq) == 4 / 8.0)
    call priority_queue_delete_top(priorityq)

    call assert(priority_queue_top_identity(priorityq) == 5)
    call assert(priority_queue_top_priority(priorityq) == 5 / 8.0)
    call priority_queue_delete_top(priorityq)

    call assert(priority_queue_top_identity(priorityq) == 6)
    call assert(priority_queue_top_priority(priorityq) == 6 / 8.0)
    call priority_queue_delete_top(priorityq)

    call assert(priority_queue_top_identity(priorityq) == 8)
    call assert(priority_queue_top_priority(priorityq) == 8 / 8.0)
    call priority_queue_delete_top(priorityq)

    call assert(.not. priority_queue_is_empty(priorityq))

    call assert(priority_queue_top_identity(priorityq) == 11)
    call assert(priority_queue_top_priority(priorityq) == 11 / 8.0)
    call priority_queue_delete_top(priorityq)

    call assert(priority_queue_is_empty(priorityq))

    print *, "Tests passed!"
end program
