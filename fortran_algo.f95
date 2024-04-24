subroutine assert(cond)
    logical, intent(in) :: cond

    if (.not. cond) then
        print *, 'Assertion failed'
        call abort()
    end if
end subroutine

module disjointset
    implicit none
    private

    public disjoint_set, disjoint_set_init, disjoint_set_find, disjoint_set_union

    type :: disjoint_set
        private
        integer, allocatable :: parent(:), rank(:)
    end type disjoint_set

contains
    function disjoint_set_init(size) result (set)
        integer, intent(in) :: size
        type(disjoint_set) :: set
        integer :: i

        allocate(set%parent(size))
        allocate(set%rank(size))
        set%rank(:) = 1

        do i = 1, size
            set%parent(i) = i
        end do
    end function disjoint_set_init

    recursive function disjoint_set_find(set, vertex) result(component)
        type(disjoint_set), intent(inout) :: set
        integer, intent(in) :: vertex
        integer :: component

        if (set%parent(vertex) /= vertex) then
            set%parent(vertex) = disjoint_set_find(set, set%parent(vertex))
        end if
        component = set%parent(vertex)
    end function disjoint_set_find


    subroutine disjoint_set_union(set, v1, v2)
        type(disjoint_set), intent(inout) :: set
        integer, intent(in) :: v1, v2
        integer :: c1, c2

        c1 = disjoint_set_find(set, v1)
        c2 = disjoint_set_find(set, v2)

        if (c1 /= c2) then
            if (set%rank(c1) < set%rank(c2)) then
                set%parent(c1) = c2
                set%rank(c2) = set%rank(c2) + set%rank(c1)
            else
                set%parent(c2) = c1
                set%rank(c1) = set%rank(c1) + set%rank(c2)
            end if
        end if
    end subroutine disjoint_set_union
end module disjointset

program algo
    use disjointset
    implicit none

    type(disjoint_set) :: set
    integer :: i, j
    set = disjoint_set_init(100)

    do i = 0, 100
        do j = 0, 100
            call assert((disjoint_set_find(set, i) == disjoint_set_find(set, j)) .eqv. (i == j));
        end do
    end do
    do i = 0, 100
        call disjoint_set_union(set, i, mod(i + 10, 100))
    end do
    do i = 0, 100
        do j = 0, 100
            call assert((disjoint_set_find(set, i) == disjoint_set_find(set, j)) .eqv. (mod(i, 10) == mod(j, 10)))
        end do
    end do
    do i = 0, 100
        call disjoint_set_union(set, 0, i)
    end do
    do i = 0, 100
        do j = 0, 100
            call assert(disjoint_set_find(set, i) == disjoint_set_find(set, j))
        end do
    end do

    print *, 'Tests passed!'
end program algo
