include "assert.f95"

module hash_table_library
    implicit none
    private

    public hash_table
    public hash_table_init, hash_table_put, hash_table_has, hash_table_get
    public hash_table_delete

    type :: element
        private
        character(32) :: key
        real :: value
        logical :: exists = .false.
    end type element

    type :: hash_table
        private
        type(element), allocatable :: data(:)
        integer :: count
    end type hash_table

contains
    function hash_string(key) result (hash)
        character(32), intent(in) :: key
        integer :: hash, i

        hash = 5381
        do i = 1, len(key)
            hash = xor(ishft(hash, 5) + hash, ichar(key(i:i)))
        end do
        hash = abs(hash)
    end function

    function lookup(tab, key) result (i)
        type(hash_table), intent(in) :: tab
        character(32), intent(in) :: key

        integer :: i
        i = mod(hash_string(key), size(tab%data)) + 1

        do while (tab%data(i)%exists .and. tab%data(i)%key /= key)
            i = mod(i, size(tab%data)) + 1
        end do
    end function

    subroutine store(tab, key, val)
        type(hash_table), intent(inout) :: tab
        character(32), intent(in) :: key
        real, intent(in) :: val

        integer :: i
        i = lookup(tab, key)

        if (.not. tab%data(i)%exists) then
            tab%data(i)%key = key
            tab%data(i)%exists = .true.
            tab%count = tab%count + 1
        end if

        tab%data(i)%value = val
    end subroutine

    subroutine resize(tab, new_size)
        type(hash_table), intent(inout) :: tab
        integer, intent(in) :: new_size

        type(hash_table) :: tmp
        integer :: i

        allocate(tmp%data(new_size))
        tmp%count = 0

        do i = 1, size(tab%data)
            if (tab%data(i)%exists) then
                call store(tmp, tab%data(i)%key, tab%data(i)%value)
            end if
        end do

        tab%count = tmp%count
        deallocate(tab%data)
        call move_alloc(tmp%data, tab%data)
    end subroutine

    function hash_table_init() result (tab)
        type(hash_table) :: tab

        allocate(tab%data(16))
        tab%count = 0
    end function

    subroutine hash_table_put(tab, key, val)
        type(hash_table), intent(inout) :: tab
        character(32), intent(in) :: key
        real, intent(in) :: val

        if (tab%count > size(tab%data) / 2) then
            call resize(tab, size(tab%data) * 2)
        end if

        call store(tab, key, val)
    end subroutine

    function hash_table_has(tab, key) result (has)
        type(hash_table), intent(inout) :: tab
        character(32), intent(in) :: key
        logical :: has

        has = tab%data(lookup(tab, key))%exists
    end function

    function hash_table_get(tab, key) result (res)
        type(hash_table), intent(inout) :: tab
        character(32), intent(in) :: key
        real :: res

        res = tab%data(lookup(tab, key))%value
    end function

    function find_first_gap(tab, begin) result (index)
        type(hash_table), intent(inout) :: tab
        integer, intent(in) :: begin
        integer :: index

        index = begin - 1
        do while (tab%data(mod(index, size(tab%data)) + 1)%exists)
            index = index + 1
        end do
    end function

    subroutine remove_block(tab, begin, preserved)
        type(hash_table), intent(inout) :: tab
        integer, intent(in) :: begin
        type(element), allocatable, intent(out) :: preserved(:)

        integer :: end, before_wrap, after_wrap

        end = find_first_gap(tab, begin)
        if (end < begin) then
            return
        else if (end == begin) then
            tab%data(begin)%exists = .false.
            tab%count = tab%count - 1
            return
        end if

        if (end <= size(tab%data)) then
            before_wrap = end - begin
            after_wrap = 0
        else
            before_wrap = size(tab%data) - begin
            after_wrap = end - size(tab%data)
        end if

        tab%count = tab%count - (1 + end - begin)
        allocate(preserved(end - begin))

        preserved(1:before_wrap) = tab%data(begin+1:begin+before_wrap)
        tab%data(begin:begin+before_wrap)%exists = .false.
        preserved(before_wrap+1:) = tab%data(1:after_wrap)
        tab%data(1:after_wrap)%exists = .false.
    end subroutine

    subroutine hash_table_delete(tab, key)
        type(hash_table), intent(inout) :: tab
        character(32), intent(in) :: key

        type(element), allocatable :: preserved(:)
        integer :: i

        if (tab%count < size(tab%data) / 8 .and. size(tab%data) >= 16) then
            call resize(tab, size(tab%data) / 2)
        end if

        call remove_block(tab, lookup(tab, key), preserved)

        if (allocated(preserved)) then
            do i = 1, size(preserved)
                call store(tab, preserved(i)%key, preserved(i)%value)
            end do
        end if
    end subroutine

end module

program hash_table_test
    use hash_table_library
    implicit none

    type(hash_table) :: table
    character(117) :: str
    integer :: i

    table = hash_table_init()
    str = "QWERTYUIOP 123456789 Warum können die Österreicher nicht Deutsch sprechen? Weil sie nur zur Schule gehen! 987654321"

    do i = 1, 60
        call assert(.not. hash_table_has(table, str(i:i+31)))
        call hash_table_put(table, str(i:i+31), i * 10.0)
        call assert(hash_table_has(table, str(i:i+31)))
        call assert(hash_table_get(table, str(i:i+31)) == i * 10.0)
    end do

    do i = 1, 60, 2
        call hash_table_delete(table, str(i:i+31))
        call assert(.not. hash_table_has(table, str(i:i+31)))
    end do
    do i = 2, 60, 4
        call hash_table_delete(table, str(i:i+31))
        call assert(.not. hash_table_has(table, str(i:i+31)))
    end do
    do i = 1, 60, 3
        call hash_table_delete(table, str(i:i+31))
        call assert(.not. hash_table_has(table, str(i:i+31)))
    end do
    do i = 1, 60
        if (mod(i, 2) /= 1 .and. mod(i, 4) /= 2 .and. mod(i, 3) /= 1) then
            call assert(hash_table_has(table, str(i:i+31)))
            call assert(hash_table_get(table, str(i:i+31)) == i * 10.0)
        else
            call assert(.not. hash_table_has(table, str(i:i+31)))
        end if
    end do
    print *, 'Tests passed!'
end program
