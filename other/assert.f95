subroutine assert(cond)
    logical, intent(in) :: cond

    if (.not. cond) then
        print *, 'Assertion failed'
        call abort()
    end if
end subroutine
