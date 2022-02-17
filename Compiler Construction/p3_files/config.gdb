set breakpoint pending on
set confirm off
file ./cminusminusc
break cminusminus::Err::report
commands
	where
end
break cminusminus::InternalError::InternalError
commands
	where
end

define p3
  set args p3_tests/$arg0.cminusminus -p --
  run
end
