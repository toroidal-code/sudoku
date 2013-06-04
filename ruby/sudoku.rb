#!/usr/bin/ruby
require 'pp'
require 'Set'

DIM = 9

class Cell
    attr_accessor :value, :possible
    def initialize(initial)
        @value = initial
        @possible = []
    end

    def to_s
      @value.to_s
    end

    def inspect
        @value.to_s
    end

end

class Sudoku
    EMPTY = 0
    attr_accessor :board

    def initialize(initial)
        @board = [] * DIM
        initial.each do |row|
            @board.push( row.map { |value| Cell.new(value) }.compact )
        end
    end
    
    def to_s
        # top row
        result = "\n " + '-' * (DIM * 2 + 5) + "\n"

        # board rows
        (0...DIM).each { |row|
            if row == 3 or row == 6
                result += '|' + '-' * (DIM * 2 + 5) + '|' + "\n"
                # result += '|-------+-------+-------|\n'
            end
            result += '| '
            (0...DIM).each { |col|
                if col == 3 or col == 6
                    result += '| '
                end
                if @board[row][col].value == 0
                    result += '.'
                else
                    result += @board[row][col].to_s
                end
                if col != DIM - 1
                    result += ' '
                end
            }
            result += ' |' + "\n"
        }

        # bottom row
        result += ' ' + '-' * (DIM * 2 + 5) + "\n"
        result += '  '
        result += "\n"
    end


    def values(direction, index)
        if direction == 0
            temp = []
            @board[index].each do |cell|
                temp.push(cell.value)
            end
            temp
        elsif direction == 1
            temp = []
            @board.each do |row|
                temp.push(row[index].value)
            end
            return temp
        elsif direction == 2
            temp = []
            case index
            when 0
                (0...3).each {|r| (0...3).each {|c| temp.push(@board[r][c].value)}}
            when 1
                (0...3).each {|r| (3...6).each {|c| temp.push(@board[r][c].value)}}
            when 2
                (0...3).each {|r| (6...9).each {|c| temp.push(@board[r][c].value)}}
            when 3
                (3...6).each {|r| (0...3).each {|c| temp.push(@board[r][c].value)}}
            when 4
                (3...6).each {|r| (3...6).each {|c| temp.push(@board[r][c].value)}}
            when 5
                (3...6).each {|r| (6...9).each {|c| temp.push(@board[r][c].value)}}
            when 6
                (6...9).each {|r| (0...3).each {|c| temp.push(@board[r][c].value)}}
            when 7
                (6...9).each {|r| (3...6).each {|c| temp.push(@board[r][c].value)}}
            when 8
                (6...9).each {|r| (6...9).each {|c| temp.push(@board[r][c].value)}}
            end
            return temp
        end
    end

    def is_valid?
        for idx in 0...DIM
            for direc in 0...3
                vals = self.values(direc, idx)
                for num in 1...(DIM + 1)
                    if vals.count(num) > 1
                        return false
                    end
                end
            end
        end
        true
    end

    def is_goal?
        @board.each do |row|
            if row.map{|c| c.value}.count(0) != 0
                return false
            end
        end
        self.is_valid?
    end

    def print_poss
        @board.each do |row|
            pp row.map{ |cell| cell.possible }
        end
    end

    def gen_poss!
        (0...DIM).each { |row|
            (0...DIM).each { |col|
                st = Set.new(self.values(0, row)).merge((self.values(1, col)).to_set.merge((self.values(2, get_quadrant(row, col))).to_set))
                if @board[row][col].value == 0
                    @board[row][col].possible = ((1...10).to_set - st).to_a
                else
                    @board[row][col].possible = []
                end
            }
        }
    end

    def process!
        self.gen_poss!
        while self.get_min_poss == 1
            (0...DIM).each { |row|
                (0...DIM).each { |col|
                    poss = @board[row][col].possible
                    if poss.size == 1
                        @board[row][col].value = poss[0]
                    end
                }
            }
            self.gen_poss!
        end
        self
    end

    def get_min_poss
        l = DIM
        (0...DIM).each { |row|
            (0...DIM).each { |col|
                poss = @board[row][col].possible
                if poss.size < l and poss.size != 0
                    l = poss.size
                end
            }
        }
        l
    end

    def get_successors
        suc = []
        (0...DIM).each { |row|
            (0...DIM).each { |col|
                if @board[row][col].value == 0 && @board[row][col].possible.size == self.get_min_poss
                    @board[row][col].possible.each do |i|
                        new_config = Marshal.load(Marshal.dump(self)) #self.dup
                        new_config.board[row][col].value = i
                        new_config.process!
                                                                     # puts "Self:\n" + self.to_s
                                                                     # puts "Clone:\n" + newConfig.to_s
                        suc.push(new_config)
                    end
                    return suc
                end
            }
        }
        []
    end

    def solve!(debug)
        if self.is_goal?
            self
        else
            if debug 
                puts("Current:\n" + self.to_s)
            end
            for successor in self.get_successors
                if self.is_valid?
                    if debug 
                        puts "Valid Successor:\n" + successor.to_s
                    end
                    solution = successor.solve!(debug)
                    if solution != nil
                        return solution
                    end
                end
            end
        end
    end


end


def get_quadrant(row, col)
    if row < 3
        if col < 3
            return 0
        elsif 3 <= col and col < 6
            return 1
        elsif col >= 6
            return 2
        end
    elsif 3 <= row and row < 6
        if col < 3
            return 3
        elsif 3 <= col and col < 6
            return 4
        elsif col >= 6
            return 5
        end
    elsif row >= 6
        if col < 3
            return 6
        elsif 3 <= col and col < 6
            return 7
        elsif col >= 6
            return 8
        end
    end
end

def read_board(filename)
    board = []
    File.open(filename, 'r').each_line { |line| board.push(line.split.map { |c| c.to_i }) }
    Sudoku.new(board)
end

sudoku = read_board('/Users/Katherine/git/sudoku/su0.txt')
puts sudoku.process!
# puts sudoku
# sudoku.getSuccessors.each {|s| puts s}
puts sudoku.solve! false
