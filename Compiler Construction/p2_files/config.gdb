set breakpoint pending on
set confirm off
file ./cronac
break lake::Err::report
commands
	where
end
break crona::ToDoError::ToDoError
commands
	where
end
break crona::InternalError::InternalError
commands
	where
end

define p2
  set args p2_tests/$arg0.crona -p --
  run
end
