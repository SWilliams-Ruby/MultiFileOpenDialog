module SW
  module MultiFileOpenDialog
    def self.entry()
      args = [ "LAZ/LAS", "*.laz;*.las", "Text", "*.txt", "All Files", "*.*" ]
      puts files = show_file_open_dialog( args )
    end
    
    # Open the file picker application
    #
    def self.show_file_open_dialog(args)
      raise "An Array of File Specification(s) is required" unless args.is_a?(Array) && args.size >= 2 

      filepaths = []
      env = {}
      prog = File.join( SW::MultiFileOpenDialog::PLUGIN_DIR, 'MultiFileOpenDialog/debug/MultiFileOpenDialog.exe' )
      cmd  = [env, prog, *args, :err=>[:child, :out] ]

      IO.popen(cmd, mode = 'a+') { |stream|
        until stream.eof? do
          filepath = stream.readline
          filepaths << filepath
        end
        
        #  child process exit status.  
        #  0 is succuss, 1 is failure
        wait_thr = Process.detach(stream.pid)          
        process_status = wait_thr.value
        exit_status = process_status.exitstatus
        if exit_status != 0
          puts "Exit Status: #{exit_status}"
          puts filepaths  # this is the error message
          filepaths = []
        end
      }
      puts "Number of files: #{ filepaths.size }"
      filepaths
    end
  end
end
