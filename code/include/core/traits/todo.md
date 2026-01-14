

# constants
- [ ] integral_constant
- [ ] bool_constant
- [ ] true_type
- [ ] false_type

# categories
- [x] is_void
- [ ] is_null_pointer
- [x] is_array
- [x] is_pointer
- [ ] is_enum
- [ ] is_union
- [ ] is_class
- [ ] is_function
- [ ] is_reference
- [ ] is_object
- [ ] is_scalar
- [ ] is_compound
- [x] is_integral
- [x] is_floating_point
- [ ] is_fundamental
- [x] is_arithmetic
- [x] is_lvalue_reference
- [ ] is_rvalue_reference
- [ ] is_member_pointer
- [ ] is_member_object_pointer
- [ ] is_member_function_pointer


# properties
- [ ] is_const
- [ ] is_volatile
- [x] is_empty
- [ ] is_polymorphic
- [ ] is_final
- [ ] is_abstract
- [ ] is_aggregate
- [ ] is_implicit_lifetime
- [ ] is_trivial
- [ ] is_trivially_copyable
- [ ] is_standard_layout
- [ ] is_literal_type
- [ ] is_pod
- [ ] is_signed
- [ ] is_unsigned
- [ ] is_bounded_array
- [ ] is_unbounded_array
- [ ] is_scoped_enum
- [ ] has_unique_object_representations



# metafunctions
- [ ] conjunction
- [ ] disjunction
- [ ] negation

	
# operations
- [ ] is_constructible
- [ ] is_trivially_constructible
- [ ] is_nothrow_constructible
- [ ] is_default_constructible
- [ ] is_trivially_default_constructible
- [ ] is_nothrow_default_constructible
- [ ] is_copy_constructible
- [ ] is_trivially_copy_constructible
- [ ] is_nothrow_copy_constructible
- [ ] is_move_constructible
- [ ] is_trivially_move_constructible
- [ ] is_nothrow_move_constructible
- [ ] is_assignable
- [ ] is_trivially_assignable
- [ ] is_nothrow_assignable
- [ ] is_copy_assignable
- [ ] is_trivially_copy_assignable
- [ ] is_nothrow_copy_assignable
- [ ] is_move_assignable
- [ ] is_trivially_move_assignable
- [ ] is_nothrow_move_assignable
- [ ] is_destructible
- [ ] is_trivially_destructible
- [ ] is_nothrow_destructible
- [ ] has_virtual_destructor
- [ ] is_swappable_with
- [ ] is_swappable
- [ ] is_nothrow_swappable_with
- [ ] is_nothrow_swappable


# relations
- [x] is_same
- [ ] is_convertible
- [ ] is_nothrow_convertible
- [ ] is_layout_compatible
- [ ] is_pointer_interconvertible_base_of
- [ ] is_pointer_interconvertible_with_class
- [ ] is_corresponding_member
- [ ] reference_constructs_from_temporary
- [ ] reference_converts_from_temporary
- [ ] is_base_of
- [ ] is_virtual_base_of
- [ ] alignment_of
- [ ] rank
- [x] extent
- [ ] is_invocable
- [ ] is_invocable_r
- [ ] is_nothrow_invocable
- [ ] is_nothrow_invocable_r


# modifiers
- [x] remove_const
- [x] remove_volatile
- [x] remove_pointer
- [x] remove_reference
- [x] remove_extent
- [ ] remove_all_extents
- [x] remove_cv
- [ ] add_const
- [ ] add_volatile
- [ ] add_pointer
- [ ] add_lvalue_reference
- [ ] add_rvalue_reference
- [ ] add_cv
- [ ] make_signed
- [ ] make_unsigned


# transformations
- [ ] aligned_storage
- [ ] aligned_union
- [ ] decay
- [x] remove_cvref
- [ ] result_of
- [ ] invoke_result
- [x] conditional
- [ ] common_type
- [ ] common_reference
- [ ] underlying_type
- [ ] type_identity
- [ ] enable_if
- [ ] void_t


# sequences
- [ ] integer_sequence
